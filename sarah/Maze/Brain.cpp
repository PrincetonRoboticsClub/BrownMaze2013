#include "Brain.h"
#include <math.h>

void Brain::setValues() {
	maz.setValues(0, 15);
	solved = false;
	reset = false;
}

void Brain::solveMaze() {
	MazeNode *next;
	MazeNode *current;
	int x;
	int y;
	int dir;

	for (; ;) {
		if (reset) { // how to reset????
			// pause for replacement
			startFromBeginning();
			reset = false;
		}

		current = maz.getCurrentNode();
		if (current == maz.getTargetNode()) {
			solved = true;
			return;
		}
		x = current->getXCoor();
		y = current->getYCoor();

		getWalls();

		next = maz.nextNodeAStar();

		if (dir = maz.isAdjacent(x, y, next->getXCoor(), next->getYCoor())) {
			// moveDir(dir);
		}
		else {
			int path[256];
			int length;
			getPath(x, y, next->getXCoor(), next->getYCoor(), &length, path);
			travelPath(&length, path);
		}
	}
}

void Brain::setReset() {
	reset = true;
}

void Brain::travelPath(int *length, int path[]) {
	int dir;
	int m;
	for (int i = 0; i < *length; ) {
		dir = path[i];
		m = 1;
		while (i < *length - 1 && path[++i] == dir) {
			m++;
		}
		// drive(int dir, int num);
	}
}


void Brain::moveDir(int dir) {
	switch(dir) {
		// case -2: drive(west); break;
		// case -1: drive(south); break;
		// case 1: drive(north); break;
		// case 2: drive(east); break;
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
	printf("common dist %d\n", commonDist);

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
	// bool w[4] = {false, false, false, false};

	// if (isWall(east))
	// 	w[0] = true;
	// if (isWall(south))
	// 	w[1] = true;
	// if (isWall(west))
	// 	w[2] = true;
	// if (isWall(north))
	// 	w[3] = true;

	// maz.getCurrentNode()->updateWalls(w);
}

void Brain::travelSolutionPath() {
	int path[256];
	int length;

	maz.getAStarSolutionPath(path, &length);

	travelPath(path, &length);
}

Maze *Brain::getMaze() {
	return &maz;
}

// int main(void) {
// 	Brain b;
// 	b.setValues();
// 	b.solveMaze();

// 	int length = b.getMaze()->getTargetNode()->getStartDist();
// 	int path[length];

// 	b.getPathToStart(&length, path, b.getMaze()->getTargetNode());
// 	b.travelPath(&length, path);

// 	b.travelSolutionPath();
// }