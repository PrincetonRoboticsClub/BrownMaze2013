#include "Brain.h"
#include <math.h>

void Brain::setValues(Robot *in, WallSensor *wsin) {
	maz.setValues(0, 15);
	solved = false;
	r = in;
	ws = wsin;
}

void Brain::solveMaze() {
	MazeNode *next;
	MazeNode *current;
	int dir;

	for (; ;) {

		current = maz.getCurrentNode();
		if (current == maz.getTargetNode()) {
			solved = true;
			return;
		}

		// getWalls();

		next = maz.nextNodeAStar();

		if (dir = maz.isAdjacent(current->getXCoor(), current->getYCoor(), next->getXCoor(), next->getYCoor())) {
			r->moveDirection(getRobotDir(dir), m);
		}
		else {
			int path[50];
			int length;
			getPath(current->getXCoor(), current->getYCoor(), next->getXCoor(), next->getYCoor(), &length, path);
			travelPath(&length, path);
		}
	}
}

void Brain::travelPath(int *length, int path[]) {
	int dir;
	uint8_t m;

	for (int i = 0; i < *length; ) {
		dir = path[i];
		m = 1;
		i++;

		while (i < *length && path[i] == dir) {
			m++;
			i++;
		}
		r->moveDirection(getRobotDir(dir), m);
	}
}

int Brain::getRobotDir(int dir) {
	switch(dir) {
		case -2: return 2;
		case -1: return 1;
		case 1: return 3;
		case 2: return 0;
	}
}

void Brain::getPath(int fromX, int fromY, int toX, int toY, int *length, int *path) {
	int length1 = maz.getNode(fromX, fromY)->getStartDist();
	int length2 = maz.getNode(toX, toY)->getStartDist();
	int path1[length1];
	int path2[length2];
	MazeNode *nodes1[length1+1];
	MazeNode *nodes2[length2+1];

	int lengthMin = (int) fmin(length1, length2);
	getPathFromStart(&length1, path1, nodes1, maz.getNode(fromX, fromY));
	getPathFromStart(&length2, path2, nodes2, maz.getNode(toX, toY));

	MazeNode *common;
	int commonDist;
	for (int i = lengthMin - 1; i >= 0; i--) {
		if (nodes1[i] == nodes2[i]) {
			common = nodes1[i];
			commonDist = i;
			break;
		}
	}

	*length = (length2 - commonDist) + (length1 - commonDist);

	for (int i = *length - 1; i >= 0; i--) {
		if (i >= length1 - commonDist)
			path[i] = path2[length2 - (*length - i)];
		else 
			path[i] = - path1[length1 - i - 1];
	}
}

void Brain::startFromBeginning() {
	// go to current node from start
	int length;
	int path[256];

	int dir;
	int x;
	int y;

	getPathFromStart(&length, path, maz.getCurrentNode());
	travelPath(&length, path);
}

void Brain::getPathToStart(int *length, int path[], MazeNode* n) {
	MazeNode *tracker = n;
	*length = n->getStartDist();
	int x;
	int y;
	int dir;

	for (int i = *length - 1; i >= 0; i--) {
		x = tracker->getXCoor();
		y = tracker->getYCoor();

		for (int j = -2; j <=2; j++) {
			if (j != 0) {
				if (maz.canTravel(x, y, j) && (maz.getDirectionNode(x, y, j)->getStartDist() == i)) {
					dir = j;
					tracker = maz.getDirectionNode(x, y, j);
				}
			}
		}
		path[*length - 1 - i] = dir;
	}
}

void Brain::getPathFromStart(int *length, int path[], MazeNode *nodes[], MazeNode* n) {
	MazeNode *tracker = n;
	*length = n->getStartDist();

	int x;
	int y;
	int dir;

	for (int i = *length - 1; i >= 0; i--) {
		x = tracker->getXCoor();
		y = tracker->getYCoor();

		for (int j = -2; j <=2; j++) {
			if (j != 0) {
				if (maz.canTravel(x, y, j) && (maz.getDirectionNode(x, y, j)->getStartDist() == i)) {
					dir = -j;
					nodes[i+1] = tracker;
					tracker = maz.getDirectionNode(x, y, j);
				}
			}
		}
		path[i] = dir;
	}
	nodes[0] = maz.getStartNode();
}

void Brain::getPathFromStart(int *length, int path[], MazeNode* n) {
	MazeNode *tracker = n;
	*length = n->getStartDist();

	int x;
	int y;
	int dir;

	for (int i = *length - 1; i >= 0; i--) {
		x = tracker->getXCoor();
		y = tracker->getYCoor();

		for (int j = -2; j <=2; j++) {
			if (j != 0) {
				if (maz.canTravel(x, y, j) && (maz.getDirectionNode(x, y, j)->getStartDist() == i)) {
					dir = -j;
					tracker = maz.getDirectionNode(x, y, j);
				}
			}
		}
		path[i] = dir;
	}
}


void Brain::getWalls() {
	maz.getCurrentNode()->updateWalls(ws->getWalls(r->getDirection()));
}

void Brain::travelSolutionPath() {
	int path[256];
	int length;

	maz.getAStarSolutionPath(path, &length);

	travelPath(&length, path);
}

Maze *Brain::getMaze() {
	return &maz;
}

int main(void) {
	Brain b;
	b.setValues();
	b.solveMaze();

	// int length = b.getMaze()->getTargetNode()->getStartDist();
	// int path[length];

	// b.getPathToStart(&length, path, b.getMaze()->getTargetNode());
	// b.travelPath(&length, path);

	// b.travelSolutionPath();
}