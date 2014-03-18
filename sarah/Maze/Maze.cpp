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

/* Returns pointer adjacent node as indicated by dir to the specified coordinates */
MazeNode *Maze::getAdjacentNode(int x, int y, enum Adjacent dir) {
   if (dir == ABOVE)
      return mazeArray[getArrayIndex(x, y-1, lengthX)];
   else if (dir == BELOW)
      return mazeArray[getArrayIndex(x, y+1, lengthX)];
   else if (dir == RIGHT) 
      return mazeArray[getArrayIndex(x+1, y, lengthX)];
   else if (dir == LEFT) 
      return mazeArray[getArrayIndex(x-1, y, lengthX)];
   else
      assert(false);
}

/* Increments number of Nodes traversed */
void Maze::incrementNumOfNodesTraversed() {
   numTraversed++;
}

/* Changes position of current node */
void Maze::changeCurrentNode(int x, int y) {
   currentPosition = mazeArray[getArrayIndex(x, y, lengthX)];
}

/* Frees memory of nodes */
void Maze::freeMaze() {
   for (int i = 0; i < lengthX; i++) {
      for (int j = 0; j < lengthY; j++) {
         free(mazeArray[getArrayIndex(i, j, lengthX)]);
      }
   }  
}
   
/* Applies A Star algorithm and returns 1 if successful and 0 if unsuccessful or
   a path has already been found */
int Maze::applyAStarAlgorithm() {
   if (this->pathFound) return 0;

   MazeNode *next;

   for ( ; ; ) {
      currentPosition->incrementNumOfTraversals();
      if (currentPosition->getNumOfTraversals() == 1) {
         currentPosition->markSolution();
         numTraversed++;
      }
      else {
         currentPosition->unmarkSolution();
      }

      if (currentPosition == nodeTarget) {
         pathFound = true;
         break;
      }

      int score = INT_MAX;
      int x = currentPosition->getXCoor();
      int y = currentPosition->getYCoor();

      if (!currentPosition->rightWall() && !this->edgeOfArray(x, y, RIGHT) && getAdjacentNode(x, y, RIGHT)->getScore() < score) {
         score = getAdjacentNode(x, y, RIGHT)->getScore();
         next = getAdjacentNode(x, y, RIGHT);
      }
      if (!currentPosition->leftWall() && !this->edgeOfArray(x, y, LEFT) && getAdjacentNode(x, y, LEFT)->getScore() < score) {
         score = getAdjacentNode(x, y, LEFT)->getScore();
         next = getAdjacentNode(x, y, LEFT);
      }
      if (!currentPosition->topWall() && !this->edgeOfArray(x, y, ABOVE) && getAdjacentNode(x, y, ABOVE)->getScore() < score) {
         score = getAdjacentNode(x, y, ABOVE)->getScore();
         next = getAdjacentNode(x, y, ABOVE);
      }
      if (!currentPosition->bottomWall() && !this->edgeOfArray(x, y, BELOW) && getAdjacentNode(x, y, BELOW)->getScore() < score) {
         score = getAdjacentNode(x, y, BELOW)->getScore();
         next = getAdjacentNode(x, y, BELOW);
      }

      currentPosition = next;
   }
   
   return 1;
}

/* Only use after applying an algorithm. Finds the shortest path and 
   returns 1 if successful and 0 if unsuccessful */
enum Adjacent* Maze::findShortestSolutionPath(enum Adjacent path[], int *size, MazeNode *start, MazeNode *end) {
   MazeNode *tracker = start;
   MazeNode *next;
   MazeNode *prev;
   int newsize = 0;

   for (int i = 0; i < *size; i++) {
      if (tracker == end) {
         *size = newsize;
         return path;
      }

      int x = tracker->getXCoor();
      int y = tracker->getYCoor();

      if (!tracker->rightWall() && getAdjacentNode(x, y, RIGHT) != prev && !edgeOfArray(x, y, RIGHT) && getAdjacentNode(x, y, RIGHT)->checkSolution()) {
         path[i] = RIGHT;
         next = getAdjacentNode(x, y, RIGHT);
      }
      else if (!tracker->leftWall() && getAdjacentNode(x, y, LEFT) != prev && !edgeOfArray(x, y, LEFT) && getAdjacentNode(x, y, LEFT)->checkSolution()) {
         path[i] = LEFT;
         next = getAdjacentNode(x, y, LEFT);
      }
      else if (!tracker->topWall() && getAdjacentNode(x, y, ABOVE) != prev && !edgeOfArray(x, y, ABOVE) && getAdjacentNode(x, y, ABOVE)->checkSolution()) {
         path[i] = ABOVE;
         next = getAdjacentNode(x, y, ABOVE);
      }
      else if (!tracker->bottomWall() && getAdjacentNode(x, y, BELOW) != prev && !edgeOfArray(x, y, BELOW) && getAdjacentNode(x, y, BELOW)->checkSolution()) {
         path[i] = BELOW;
         next = getAdjacentNode(x, y, BELOW);
      }

      newsize++;
      next->markSolution();
      prev = tracker;
      tracker = next;
   }

   return 0;
}


