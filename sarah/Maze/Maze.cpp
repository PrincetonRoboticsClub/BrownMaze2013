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

bool Maze::edgeOfArray(int x, int y, enum Adjacent dir) {
   if (dir == ABOVE) {
      if (y == 0) return true;
   }
   else if (dir == BELOW) {
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
         mazeArray[getArrayIndex(i, j, lengthX)]->setValues(i, j, abs(i - targetX) + abs(j - targetY), lengthX + lengthY, w);
      }
   }  

   /* pointers to the start, target, and current nodes in the maze array */
   nodeStart = mazeArray[getArrayIndex(startX, startY, lengthX)];
   nodeTarget = mazeArray[getArrayIndex(targetX, targetY, lengthX)];
   currentPosition = nodeStart;

   /* number of nodes that have been traversed */
   numTraversed = 0;
   
   /* whether or not a path has been found */
   pathFound = false;
}

/* Returns the number of nodes traversed */
int Maze::getNumOfNodesTraversed() {
   return numTraversed;
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

/* Returns pointer node above (lower Y coordinate) specified coordinates */
MazeNode *Maze::getNodeAbove(int x, int y) {
   return mazeArray[getArrayIndex(x, y-1, lengthX)];
}
   
/* Returns pointer node below (higher Y coordinate)  specified coordinates */
MazeNode *Maze::getNodeBelow(int x, int y) {
   return mazeArray[getArrayIndex(x, y+1, lengthX)];
}

/* Returns pointer node to the right (higher X coordinate) of specified coordinates */
MazeNode *Maze::getNodeRight(int x, int y) {
   return mazeArray[getArrayIndex(x+1, y, lengthX)];
}

/* Returns pointer node to the left (lower X coordinate) of specified coordinates */
MazeNode *Maze::getNodeLeft(int x, int y) {
   return mazeArray[getArrayIndex(x-1, y, lengthX)];
}

/* Increments number of Nodes traversed */
void Maze::incrementNumOfNodesTraversed() {
   numTraversed++;
}
   
/* Applies A Star algorithm and returns 1 if successful and 0 if unsuccessful or
   a path has already been found */
int Maze::applyAStarAlgorithm() {
   if (this->pathFound) return 0;

   for ( ; ; ) {
      currentPosition->incrementNumOfTraversals();
      if (currentPosition == nodeTarget) {
         pathFound = true;
         break;
      }

      MazeNode *next = nodeStart;
      int score = INT_MAX;
      int x = currentPosition->getXCoor();
      int y = currentPosition->getYCoor();

      if (!currentPosition->rightWall() && !this->edgeOfArray(x, y, RIGHT) && getNodeRight(x, y)->getScore() < score) {
         score = getNodeRight(x, y)->getScore();
         next = this->getNodeRight(x, y);
      }
      if (!currentPosition->leftWall() && !this->edgeOfArray(x, y, LEFT) && getNodeLeft(x, y)->getScore() < score) {
         score = getNodeLeft(x, y)->getScore();
         next = this->getNodeLeft(x, y);
      }
      if (!currentPosition->topWall() && !this->edgeOfArray(x, y, ABOVE) && getNodeAbove(x, y)->getScore() < score) {
         score = getNodeAbove(x, y)->getScore();
         next = this->getNodeAbove(x, y);
      }
      if (!currentPosition->bottomWall() && !this->edgeOfArray(x, y, BELOW) && getNodeBelow(x, y)->getScore() < score) {
         score = getNodeBelow(x, y)->getScore();
         next = this->getNodeBelow(x, y);
      }

      if (currentPosition->getNumOfTraversals() == 1) 
         numTraversed++;
      currentPosition = next;
      printf("current position %d %d\n", currentPosition->getXCoor(), currentPosition->getYCoor());
   }
   
   return 1;
}

/* Applies Tremaux algorithm and returns 1 if successful and 0 if unsuccessful */
int Maze::applyTremauxAlgorithm() {
   return 0;
}

/* Only use after applying an algorithm. Finds the shortest path and 
   returns 1 if successful and 0 if unsuccessful */
int Maze::findShortestSolutionPath() {
   return 0;
}

void Maze::changeCurrentNode(int x, int y) {
   currentPosition = mazeArray[getArrayIndex(x, y, lengthX)];
}

void Maze::freeMaze() {
   for (int i = 0; i < lengthX; i++) {
      for (int j = 0; j < lengthY; j++) {
         free(mazeArray[getArrayIndex(i, j, lengthX)]);
      }
   }  
}
