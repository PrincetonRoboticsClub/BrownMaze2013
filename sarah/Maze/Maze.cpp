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
   getNode(0, 0)->setStartDist(0);

   /* pointers to the start, target, and current nodes in the maze array */
   nodeStart = mazeArray[getArrayIndex(startX, startY, lengthX)];
   nodeTarget = mazeArray[getArrayIndex(targetX, targetY, lengthX)];
   currentPosition = nodeStart;

   /* number of nodes that have been traversed */
   numTraversed = 0;
   
   /* whether or not a path has been found */
   pathFound = false;
   path = (enum Adjacent *) malloc(sizeof(enum Adjacent)*lengthX*lengthY);
   pathLength = 0;
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

enum Adjacent *Maze::getPath(int *length) {
   *length = pathLength;
   return path;
}

void Maze::addToPath(enum Adjacent val) {
   path[pathLength] = val;
   pathLength++;
}

MazeNode *Maze::nextNodeAStar() {
   if (this->pathFound) return NULL;
   MazeNode *next;


      currentPosition->incrementNumOfTraversals();
      if (currentPosition->getNumOfTraversals() == 1) {
         currentPosition->markSolution();
         numTraversed++;
      }
      /* else {
         currentPosition->unmarkSolution(); 
      } */

      if (currentPosition == nodeTarget) {
         pathFound = true;
         return NULL;
      }

      enum Adjacent dir;
      double score = 100000.0;
      int x = currentPosition->getXCoor();
      int y = currentPosition->getYCoor();

      if (!currentPosition->hasWall(RIGHT_W) && !this->edgeOfArray(x, y, RIGHT)) { 
         if (getAdjacentNode(x, y, RIGHT)->getStartDist() + 1 < currentPosition->getStartDist()) 
            currentPosition->setStartDist(getAdjacentNode(x, y, RIGHT)->getStartDist() + 1);
      }
      if (!currentPosition->hasWall(LEFT_W) && !this->edgeOfArray(x, y, LEFT)) { 
         if (getAdjacentNode(x, y, LEFT)->getStartDist() + 1 < currentPosition->getStartDist()) 
            currentPosition->setStartDist(getAdjacentNode(x, y, LEFT)->getStartDist() + 1);
      }
      if (!currentPosition->hasWall(ABOVE_W) && !this->edgeOfArray(x, y, UP)) { 
         if (getAdjacentNode(x, y, UP)->getStartDist() + 1 < currentPosition->getStartDist()) 
            currentPosition->setStartDist(getAdjacentNode(x, y, UP)->getStartDist() + 1);
      }
      if (!currentPosition->hasWall(BELOW_W) && !this->edgeOfArray(x, y, DOWN)) {
         if (getAdjacentNode(x, y, DOWN)->getStartDist() + 1 < currentPosition->getStartDist()) 
            currentPosition->setStartDist(getAdjacentNode(x, y, DOWN)->getStartDist() + 1);
      }

      if (!currentPosition->hasWall(RIGHT_W) && !this->edgeOfArray(x, y, RIGHT)) {
         if (currentPosition->getStartDist() + 1 < getAdjacentNode(x, y, RIGHT)->getStartDist()) {
            getAdjacentNode(x, y, RIGHT)->setStartDist(currentPosition->getStartDist() + 1);
         } 
         if (getAdjacentNode(x, y, RIGHT)->getScore() < score || (getAdjacentNode(x, y, RIGHT)->getScore() == score && getAdjacentNode(x, y, RIGHT)->getManhattanDist() < next->getManhattanDist())) {
            score = getAdjacentNode(x, y, RIGHT)->getScore();
            next = getAdjacentNode(x, y, RIGHT);
            dir = RIGHT;
         }
      }
      if (!currentPosition->hasWall(LEFT_W) && !this->edgeOfArray(x, y, LEFT)) { 
         if (currentPosition->getStartDist() + 1 < getAdjacentNode(x, y, LEFT)->getStartDist()) {
            getAdjacentNode(x, y, LEFT)->setStartDist(currentPosition->getStartDist() + 1);
         } 
         if (getAdjacentNode(x, y, LEFT)->getScore() < score || (getAdjacentNode(x, y, LEFT)->getScore() == score && getAdjacentNode(x, y, LEFT)->getManhattanDist() < next->getManhattanDist())) {
            score = getAdjacentNode(x, y, LEFT)->getScore();
            next = getAdjacentNode(x, y, LEFT);
            dir = LEFT;
         }
      }
      if (!currentPosition->hasWall(ABOVE_W) && !this->edgeOfArray(x, y, UP)) { 
         if (currentPosition->getStartDist() + 1 < getAdjacentNode(x, y, UP)->getStartDist()) {
            getAdjacentNode(x, y, UP)->setStartDist(currentPosition->getStartDist() + 1);
         } 
         if (getAdjacentNode(x, y, UP)->getScore() < score || (getAdjacentNode(x, y, UP)->getScore() == score && getAdjacentNode(x, y, UP)->getManhattanDist() < next->getManhattanDist())) {
            score = getAdjacentNode(x, y, UP)->getScore();
            next = getAdjacentNode(x, y, UP);
            dir = UP;
         }
      }
      if (!currentPosition->hasWall(BELOW_W) && !this->edgeOfArray(x, y, DOWN)) {
         if (currentPosition->getStartDist() + 1 < getAdjacentNode(x, y, DOWN)->getStartDist()) {
            getAdjacentNode(x, y, DOWN)->setStartDist(currentPosition->getStartDist() + 1);
         } 
         if (getAdjacentNode(x, y, DOWN)->getScore() < score || (getAdjacentNode(x, y, DOWN)->getScore() == score && getAdjacentNode(x, y, DOWN)->getManhattanDist() < next->getManhattanDist())) {
            score = getAdjacentNode(x, y, DOWN)->getScore();
            next = getAdjacentNode(x, y, DOWN);
            dir = DOWN;
         }
      }

      currentPosition = next;
      path[pathLength] = dir;
      pathLength++;

      return currentPosition;
}
 
/* Applies A Star algorithm */
void Maze::applyAStarAlgorithm() {
   for (; ;) {
      if (pathFound) return;
      nextNodeAStar();
   }
}


