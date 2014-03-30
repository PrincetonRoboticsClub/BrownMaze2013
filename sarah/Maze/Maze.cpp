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

/* Tests if the node in the direction indicated from the given coordinates can be accessed. 
   Determine if there is a wall and/or if it is at the edge of the maze. Returns true if
   can travel in the indicated direction and false otherwise */
bool Maze::canTravel(int x, int y, enum Direction dir) {
   // Tests for wall
   if (getNode(x, y)->hasWall(dir))
      return false;

   // Tests if it is at the edge of the array
   if (dir == UP) {
      if (y == 0) return false;
   }
   else if (dir == DOWN) {
      if (y == lengthY - 1) return false;
   } 
   else if (dir == RIGHT) {
      if (x == lengthX - 1) return false;
   }
   else if (dir == LEFT) {
      if (x == 0) return false;
   }
   return true;
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

   /* number of nodes that have been traversed */
   numTraversed = 0;
   
   /* whether or not a path has been found */
   solutionFound = false;
   path = (enum Direction *) malloc(sizeof(enum Direction)*lengthX*lengthY);
   pathLength = 0; // length of path which IS NOT the number of nodes in the path (always 1 less than number of nodes in path)

   nodeStart->setStartDist(0);
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

/* Should only be called after A Star has been applied */
enum Direction *Maze::getAStarSolutionPath(int *length) {
   if (pathLength > 0) {
      *length = pathLength;
      return path;
   }

   enum Direction val;
   MazeNode *tracker = nodeTarget;
   pathLength = nodeTarget->getStartDist();
   *length = pathLength;

   for (int i = pathLength - 1; i >= 0; i--) {
      int x = tracker->getXCoor();
      int y = tracker->getYCoor();
 
      if (this->canTravel(x, y, RIGHT) && (getDirectionNode(x, y, RIGHT)->getStartDist() == i)) {
         val = LEFT;
         tracker = getDirectionNode(x, y, RIGHT);
      }
      else if (this->canTravel(x, y, LEFT) && (getDirectionNode(x, y, LEFT)->getStartDist() == i)) {
         val = RIGHT;
         tracker = getDirectionNode(x, y, LEFT);
      }
      else if (this->canTravel(x, y, UP) && (getDirectionNode(x, y, UP)->getStartDist() == i)) {
         val = DOWN;
         tracker = getDirectionNode(x, y, UP);
      }
      else if (this->canTravel(x, y, DOWN) && (getDirectionNode(x, y, DOWN)->getStartDist() == i)) { 
         val = UP;
         tracker = getDirectionNode(x, y, DOWN);
      }
      path[i] = val;
   }

   return path; 
}

void Maze::updateStartDistances(int x, int y) {

   // when changing start distance, change start distance continuously for adjacent nodes (only if the node that's updating the ones around it
   // has already been seen though, otherwise could be not actually accessible) until it reaches a point where the start distance is greater than the current start distance
   // the current flaw in this code versus a star is that the start distance is not computer completely accurately

   if (this->canTravel(x, y, RIGHT)) { 
      if (getDirectionNode(x, y, RIGHT)->getStartDist() + 1 < currentPosition->getStartDist()) 
         currentPosition->setStartDist(getDirectionNode(x, y, RIGHT)->getStartDist() + 1);
   }
   if (this->canTravel(x, y, LEFT)) { 
      if (getDirectionNode(x, y, LEFT)->getStartDist() + 1 < currentPosition->getStartDist()) 
         currentPosition->setStartDist(getDirectionNode(x, y, LEFT)->getStartDist() + 1);
   }
   if (this->canTravel(x, y, UP)) { 
      if (getDirectionNode(x, y, UP)->getStartDist() + 1 < currentPosition->getStartDist()) 
         currentPosition->setStartDist(getDirectionNode(x, y, UP)->getStartDist() + 1);
   }
   if (this->canTravel(x, y, DOWN)) {
      if (getDirectionNode(x, y, DOWN)->getStartDist() + 1 < currentPosition->getStartDist()) 
         currentPosition->setStartDist(getDirectionNode(x, y, DOWN)->getStartDist() + 1);
   }

   if (this->canTravel(x, y, RIGHT)) {
      if (currentPosition->getStartDist() + 1 < getDirectionNode(x, y, RIGHT)->getStartDist()) {
         getDirectionNode(x, y, RIGHT)->setStartDist(currentPosition->getStartDist() + 1);
      } 
   }
   if (this->canTravel(x, y, LEFT)) { 
      if (currentPosition->getStartDist() + 1 < getDirectionNode(x, y, LEFT)->getStartDist()) {
         getDirectionNode(x, y, LEFT)->setStartDist(currentPosition->getStartDist() + 1);
      } 
   }
   if (this->canTravel(x, y, UP)) { 
      if (currentPosition->getStartDist() + 1 < getDirectionNode(x, y, UP)->getStartDist()) {
         getDirectionNode(x, y, UP)->setStartDist(currentPosition->getStartDist() + 1);
      } 
   }
   if (this->canTravel(x, y, DOWN)) {
      if (currentPosition->getStartDist() + 1 < getDirectionNode(x, y, DOWN)->getStartDist()) {
         getDirectionNode(x, y, DOWN)->setStartDist(currentPosition->getStartDist() + 1);
      } 
   }
}

MazeNode *Maze::nextNodeAStar() {
   // account for whether or not it has been traversed
   if (this->solutionFound) return NULL;
   MazeNode *next;

   currentPosition->incrementNumOfTraversals();
   if (currentPosition->getNumOfTraversals() == 1) {
      currentPosition->markSolution();
      numTraversed++;
   }

   if (currentPosition == nodeTarget) {
      solutionFound = true;
      return NULL;
   }

   double score = 100000.0;
   int x = currentPosition->getXCoor();
   int y = currentPosition->getYCoor();

   updateStartDistances(x, y);


   if (this->canTravel(x, y, RIGHT)) {
      if (hasBetterScore(RIGHT, x, y, score, next)) {
         score = getDirectionNode(x, y, RIGHT)->getScore();
         next = getDirectionNode(x, y, RIGHT);
      }
   }
   if (this->canTravel(x, y, LEFT)) { 
      if (hasBetterScore(LEFT, x, y, score, next)) {
         score = getDirectionNode(x, y, LEFT)->getScore();
         next = getDirectionNode(x, y, LEFT);
      }
   }
   if (this->canTravel(x, y, UP)) { 
      if (hasBetterScore(UP, x, y, score, next)) {
         score = getDirectionNode(x, y, UP)->getScore();
         next = getDirectionNode(x, y, UP);
      }
   }
   if (this->canTravel(x, y, DOWN)) {
      if (hasBetterScore(DOWN, x, y, score, next)) {
         score = getDirectionNode(x, y, DOWN)->getScore();
         next = getDirectionNode(x, y, DOWN);
      }
   }

   currentPosition = next;
   return currentPosition;
}

bool Maze::hasBetterScore(enum Direction dir, int x, int y, int score, MazeNode *next) {
   if (getDirectionNode(x, y, dir)->getScore() < score || 
      (getDirectionNode(x, y, dir)->getScore() == score 
         && getDirectionNode(x, y, dir)->getManhattanDist() < next->getManhattanDist()))
      return true;
}
 
/* Applies A Star algorithm */
void Maze::applyAStarAlgorithm() {
   for (; ;) {
      if (solutionFound) return;
      nextNodeAStar();
   }
   return;
}

void Maze::applyMazeWalls(bool newwalls[][16][4], int width, int height) {
   for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
         getNode(i, j)->updateWalls(newwalls[j][i]);
      }
   }
   return;
} 


// when it makes a tradeoff that it does not explore, put it on a priority queue
