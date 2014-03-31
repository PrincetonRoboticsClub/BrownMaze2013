/******************************************************************************
 * Brown Competition 2014
 * Maze.h
 *
 * Models a rectangular maze with start and target nodes. Two algorithms,
 * A Star and Tremaux, can be used to traverse the array, after which a shortest
 * successful path can be determined. This class is dependent on the MazeNode
 * class and be used in the Brain class in order to solve the maze.
 * 
 ******************************************************************************/


#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "Maze.h"

enum {INT_MAX = 100000};

/* Returns 1-D array index x, y coordinates */
static int getArrayIndex(int x, int y, int width) {
   return width * x + y;
}

bool Maze::canTravel(int x, int y, enum Direction dir) {
   if (dir == UP) {
      if (y == 0) return true;
   }
   else if (dir == DOWN) {
      if (y == lengthY - 1) return true;
   } 
   else if (dir == RIGHT) {
      if (x == lengthX - 1) return true;
   }
   else if (dir == LEFT) {
      if (x == 0) return true;
   }
   return false;
}

/* Maze constructor takes dimensions of a rectangular maze represented
   by an array of MazeNodes startArray, and the coordinates of
   the start node and target node. The nodes are ordered so that the
   origin (0,0) node is in the top left. */
void Maze::setValues(int lenX, int lenY, int startX, int startY, int targetX, int targetY) {
   assert(lenX > 0);
   assert(lenY > 0);
   assert(startX >= 0 && startX < lenX);
   assert(startY >= 0 && startY < lenY);
   assert(targetX >= 0 && targetX < lenX);
   assert(targetY >= 0 && targetY < lenY);

   /* length of the maze in dimension x (number of columns) */
   lengthX = lenX;
   /* length of the maze in dimension y (number of rows) */
   lengthY = lenY;
   
   /* the maze is represented by an array of nodes, each node representing
      a tile within the maze. It intializes a clean maze with no walls 
      or paths traversed yet.*/	
   mazeArray = (MazeNode **) malloc(sizeof(MazeNode)*lengthX*lengthY);
   bool w[4] = {false, false, false, false};

   for (int i = 0; i < lengthX; i++) {
      for (int j = 0; j < lengthY; j++) {
         mazeArray[getArrayIndex(i, j, lengthX)] = (MazeNode *) malloc(sizeof(MazeNode));
         mazeArray[getArrayIndex(i, j, lengthX)]->setValues(i, j, abs(i - targetX) + abs(j - targetY), lengthX*lengthY, w);
      }
   }  

   /* pointers to the start, target, and current nodes in the maze array */
   nodeStart = mazeArray[getArrayIndex(startX, startY, lengthX)];
   nodeTarget = mazeArray[getArrayIndex(targetX, targetY, lengthX)];
   currentPosition = nodeStart;

   path = (enum Direction *) malloc(sizeof(enum Direction)*lengthX*lengthY);
   pathLength = 0; // length of path which IS NOT the number of nodes in the path (always 1 less than number of nodes in path)

   nodeStart->setStartDist(0);
   q.setValues(lengthX*lengthY);
}

/* Return the horizontal length (X dimension) of the maze */
int Maze::getLengthX() {
   return lengthX;
}

/* Return the vertical length (Y dimension) of the maze */
int Maze::getLengthY() {
   return lengthY;
}

/* Return size of (number of nodes in) the array */
int Maze::getSize() {
   return lengthX * lengthY;
}

/* Returns pointer to current MazeNode */
MazeNode *Maze::getCurrentNode() {
   return currentPosition;
}

/* Returns pointer to start MazeNode */
MazeNode *Maze::getStartNode() {
   return nodeStart;
}

/* Returns pointer to target MazeNode */
MazeNode *Maze::getTargetNode() {
   return nodeTarget;
}

/* Returns pointer node with specified coordinates */
MazeNode *Maze::getNode(int x, int y) {
   return mazeArray[getArrayIndex(x, y, lengthX)];
}

/* Returns pointer adjacent node as indicated by dir to the specified coordinates */
MazeNode *Maze::getDirectionNode(int x, int y, enum Direction dir) {
   if (dir == UP)
      return mazeArray[getArrayIndex(x, y-1, lengthX)];
   else if (dir == DOWN)
      return mazeArray[getArrayIndex(x, y+1, lengthX)];
   else if (dir == RIGHT) 
      return mazeArray[getArrayIndex(x+1, y, lengthX)];
   else if (dir == LEFT) 
      return mazeArray[getArrayIndex(x-1, y, lengthX)];
   else
      assert(false);
}

/* Changes position of current node */
void Maze::moveToNode(int x, int y) {
   currentPosition = mazeArray[getArrayIndex(x, y, lengthX)];
}

/* Frees memory of nodes */
void Maze::freeMaze() {
   for (int i = 0; i < lengthX; i++) {
      for (int j = 0; j < lengthY; j++) {
         free(mazeArray[getArrayIndex(i, j, lengthX)]);
      }
   }  
   q.freePQ();
}

void Maze::findDirectionNodes(MazeNode *adjNodes[4], int *size) {
   int x = currentPosition->getXCoor();

   int y = currentPosition->getYCoor();
   *size = 0;

   if (this->canTravel(x, y, RIGHT)) {
      adjNodes[*size] = getDirectionNode(x, y, RIGHT);
      (*size)++;
   }
   if (this->canTravel(x, y, LEFT)) { 
      adjNodes[*size] = getDirectionNode(x, y, LEFT);
      (*size)++;
   }
   if (this->canTravel(x, y, UP)) { 
      adjNodes[*size] = getDirectionNode(x, y, UP);
      (*size)++;
   }
   if (this->canTravel(x, y, DOWN)) {
      adjNodes[*size] = getDirectionNode(x, y, DOWN);
      (*size)++;
   }
}

MazeNode *Maze::nextNodeAStar() {
   MazeNode *adjNodes[4];
   int numAdjNodes = 0;

   findDirectionNodes(adjNodes, &numAdjNodes);
   MazeNode *N;
   for (int i = 0; i < numAdjNodes; i++) {
      N = adjNodes[i];
      if (currentPosition->getStartDist() + 1 < N->getStartDist())
            N->setStartDist(currentPosition->getStartDist() + 1);

      if (!N->checkVisited()) {
         N->markVisited();
         q.push(N);
      }
   }

   return q.pop();
}


void Maze::applyAStarAlgorithm() {
   currentPosition->markVisited();
   currentPosition->incrementNumOfTraversals();
   
   do {
      currentPosition = nextNodeAStar();
      currentPosition->incrementNumOfTraversals();
   } while (currentPosition != nodeTarget);
}

void Maze::applyMazeWalls(bool newwalls[][16][4], int width, int height) {
   for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
         getNode(i, j)->updateWalls(newwalls[j][i]);
      }
   }
   return;
} 

