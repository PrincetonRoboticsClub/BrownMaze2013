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


/* Returns 1-D array index x, y coordinates */
static int getArrayIndex(int x, int y) {
   return 16 * x + y;
}

/* Tests if the node in the direction indicated from the given coordinates can be accessed. 
   Determine if there is a wall and/or if it is at the edge of the maze. Returns true if
   can travel in the indicated direction and false otherwise 
   directions: {UP = 1, DOWN = -1, RIGHT = 2, LEFT = -2}; */
bool Maze::canTravel(int x, int y, int dir) {
   // Tests for wall
   if (getNode(x, y)->hasWall(dir))
      return false;

   // Tests if it is at the edge of the array
   if (dir == 1) { // up
      if (y == 0) return false;
   }
   else if (dir == -1) { // down
      if (y == 16 - 1) return false;
   } 
   else if (dir == 2) { // right
      if (x == 16 - 1) return false;
   }
   else if (dir == -2) { // left
      if (x == 0) return false;
   }
   return true;
}

// always assume 16 x 16 array and center as target
void Maze::setValues(char startX, char startY) {
   /* the maze is represented by an array of nodes, each node representing
      a tile within the maze. It intializes a clean maze with no walls 
      or paths traversed yet.*/	
   bool w[4] = {false, false, false, false};

   for (char i = 0; i < 16; i++) {
      for (char j = 0; j < 16; j++) {
         (mazeArray[getArrayIndex(i, j)]).setValues(i, j, 0xFF, 0x00);
      }
   }  

   nodeStart = ((0x00 | startY) << 4) | startX; // x coord is lower 4 bits, y coord is upper 4 bits
   currentPosition = nodeStart; // x coord is lower 4 bits, y coord is upper 4 bits 
   numTraversed = 0x00; // only necessary for testing, 8 bits
   mostInner = nodeStart;
   
   (mazeArray[getArrayIndex(startX, startY)]).setStartDist(0);

}

/* Returns the number of nodes traversed */
int Maze::getNumOfNodesTraversed() {
   return 0x00FF & numTraversed;
}

/* Return the horizontal length (X dimension) of the maze */
int Maze::getLengthX() {
   return 16;
}

/* Return the vertical length (Y dimension) of the maze */
int Maze::getLengthY() {
   return 16;
}

/* Return size of (number of nodes in) the array */
int Maze::getSize() {
   return 16 * 16;
}

/* Returns pointer to current MazeNode */
MazeNode *Maze::getCurrentNode() {
   return getNode(0x000F & currentPosition, (0x00F0 & currentPosition) >> 4);
}

/* Returns pointer to start MazeNode */
MazeNode *Maze::getStartNode() {
   return getNode(0x000F & nodeStart, (0x00F0 & nodeStart) >> 4);
}

/* Returns pointer to target MazeNode */
MazeNode *Maze::getTargetNode() {
   return getNode(7, 7);
}

/* Returns pointer node with specified coordinates */
MazeNode *Maze::getNode(int x, int y) {
   return &(mazeArray[getArrayIndex(x, y)]);
}

/* Returns pointer adjacent node as indicated by dir to the specified coordinates */
// directions: {UP = 1, DOWN = -1, RIGHT = 2, LEFT = -2}; 
MazeNode *Maze::getDirectionNode(int x, int y, int dir) {
   if (dir == 1) // up
      return &(mazeArray[getArrayIndex(x, y-1)]);
   else if (dir == -1) // down
      return &(mazeArray[getArrayIndex(x, y+1)]);
   else if (dir == 2) // right
      return &(mazeArray[getArrayIndex(x+1, y)]);
   else if (dir == -2) // left
      return &(mazeArray[getArrayIndex(x-1, y)]);
}

/* Increments number of Nodes traversed */
void Maze::incrementNumOfNodesTraversed() {
   numTraversed++;
}

/* Changes position of current node */
void Maze::changeCurrentNode(char x, char y) {
   currentPosition = ((0x00 | y) << 4) | x;
}

/* Should only be called after A Star has been applied */
void Maze::getAStarSolutionPath(int path[256], int *length) {
   int dir;
   MazeNode *tracker = getTargetNode();
   *length = getTargetNode()->getStartDist();

   for (int i = *length - 1; i >= 0; i--) {
      int x = tracker->getXCoor();
      int y = tracker->getYCoor();

      for (int j = -2; j <=2; j++) {
         if (j != 0) {
            if (canTravel(x, y, j) && (getDirectionNode(x, y, j)->getStartDist() == i)) {
               dir = -j;
               tracker = getDirectionNode(x, y, j);
            }
         }
      }
 
      path[i] = dir;
   }
}

void Maze::updateStartDistances(int x, int y) {
   // the current flaw in this code versus a star is that the start distance is not computer completely accurately
   MazeNode *tracker = getNode(x, y);

   // makes sure start distance of current node being considered is as small as possible

   for (int j = -2; j <=2; j++) {
      if (j != 0) {
         if (canTravel(x, y, j)) { 
            if (getDirectionNode(x, y, j)->getStartDist() + 1 < tracker->getStartDist()) {
               tracker->setStartDist(getDirectionNode(x, y, j)->getStartDist() + 1);
            }
         }
      }
   }

   if (tracker->getNumOfTraversals() < 1) return;

   for (int j = -2; j <=2; j++) {
      if (j != 0) {
         if (canTravel(x, y, j)) {
            if (tracker->getStartDist() + 1 < getDirectionNode(x, y, j)->getStartDist()) {
               updateStartDistances(getDirectionNode(x, y, j)->getXCoor(), getDirectionNode(x, y, j)->getYCoor());
            }
         }
      }
   }

   return;
}

MazeNode *Maze::nextNodeAStar() {
   if (currentPosition == 0x77) return NULL;
   MazeNode *next = getCurrentNode();
   MazeNode *current = getCurrentNode();

   current->incrementNumOfTraversals();
   if (current->getNumOfTraversals() == 1) {
      numTraversed++;
   }
   int x = current->getXCoor();
   int y = current->getYCoor();

   updateStartDistances(x, y);

   if (currentPosition == 0x77) {
      return NULL;
   }

   // check for dead end
   if (current->getNumOfOpenWalls() == 1) {
      current->markDeadEnd();
      if (canTravel(x, y, 2)) {
         current = getDirectionNode(x, y, 2);
      }
      else if (canTravel(x, y, -2)) {
         current = getDirectionNode(x, y, -2);
      }
      else if (canTravel(x, y, 1)) {
         current = getDirectionNode(x, y, 1);
      }
      else if (canTravel(x, y, -1)) {
         current = getDirectionNode(x, y, -1);
      }
      currentPosition = ((0x00 | current->getYCoor()) << 4) | current->getXCoor();
      return current;
   }
   else if (current->getNumOfOpenWalls() == 2) {
      if ((canTravel(x, y, 2) && getDirectionNode(x, y, 2)->isDeadEnd()) 
         || (canTravel(x, y, -2) && getDirectionNode(x, y, -2)->isDeadEnd())
         || (canTravel(x, y, -1) && getDirectionNode(x, y, -1)->isDeadEnd())
         || (canTravel(x, y, 1) && getDirectionNode(x, y, 1)->isDeadEnd())) {


         current->markDeadEnd();

         if (canTravel(x, y, 2) && !getDirectionNode(x, y, 2)->isDeadEnd()) {
            current = getDirectionNode(x, y, 2);
         }
         else if (canTravel(x, y, -2) && !getDirectionNode(x, y, -2)->isDeadEnd()) {
            current = getDirectionNode(x, y, -2);
         }
         else if (canTravel(x, y, 1) && !getDirectionNode(x, y, 1)->isDeadEnd()) {
            current = getDirectionNode(x, y, 1);
         }
         else if (canTravel(x, y, -1) && !getDirectionNode(x, y, -1)->isDeadEnd()) {
            current = getDirectionNode(x, y, -1);
         }
         currentPosition = ((0x00 | current->getYCoor()) << 4) | current->getXCoor();
         return current;
      }
   }
   else if (current->getNumOfOpenWalls() == 3) {
      if ((canTravel(x, y, 2) && getDirectionNode(x, y, 2)->isDeadEnd() && canTravel(x, y, 1) && getDirectionNode(x, y, 1)->isDeadEnd())
         || (canTravel(x, y, 2) && getDirectionNode(x, y, 2)->isDeadEnd() && canTravel(x, y, -1) && getDirectionNode(x, y, -1)->isDeadEnd())
         || (canTravel(x, y, 2) && getDirectionNode(x, y, 2)->isDeadEnd() && canTravel(x, y, -2) && getDirectionNode(x, y, -2)->isDeadEnd()) 
         || (canTravel(x, y, -2) && getDirectionNode(x, y, -2)->isDeadEnd() && canTravel(x, y, 1) && getDirectionNode(x, y, 1)->isDeadEnd())
         || (canTravel(x, y, -2) && getDirectionNode(x, y, -2)->isDeadEnd() && canTravel(x, y, -1) && getDirectionNode(x, y, -1)->isDeadEnd())
         || (canTravel(x, y, -1) && getDirectionNode(x, y, -1)->isDeadEnd() && canTravel(x, y, 1) && getDirectionNode(x, y, 1)->isDeadEnd())) {

         current->markDeadEnd();

         if (canTravel(x, y, 2) && !getDirectionNode(x, y, 2)->isDeadEnd()) {
            current = getDirectionNode(x, y, 2);
         }
         else if (canTravel(x, y, -2) && !getDirectionNode(x, y, -2)->isDeadEnd()) {
            current = getDirectionNode(x, y, -2);
         }
         else if (canTravel(x, y, 1) && !getDirectionNode(x, y, 1)->isDeadEnd()) {
            current = getDirectionNode(x, y, 1);
         }
         else if (canTravel(x, y, -1) && !getDirectionNode(x, y, -1)->isDeadEnd()) {
            current = getDirectionNode(x, y, -1);
         }
         currentPosition = ((0x00 | current->getYCoor()) << 4) | current->getXCoor();
         return current;
      }
   } 


   double score = 100000.0;

   // possibility of getting no node....
   // should never go to itself
   for (int j = -2; j <=2; j++) {
      if (j != 0) {
         if (canTravel(x, y, j) && getDirectionNode(x, y, j)->shouldTraverse()) {
            if (getDirectionNode(x, y, j)->getNumOfTraversals() == 0) {
               current = getDirectionNode(x, y, j);
               currentPosition = ((0x00 | current->getYCoor()) << 4) | current->getXCoor();
               return current;
            }
            if (hasBetterScore(j, x, y, score, next)) {
               score = getDirectionNode(x, y, j)->getScore(7.5, 7.5);
               next = getDirectionNode(x, y, j);
            }
         }
      }
   }


   if (current == next) {
      updateMostInner();
      next = getMostInnerTraversed();
   }
   
   // use another mazenode variable to make sure it only goes in directions it has not gone before
   // use structure to track loops --> each "loop" has a certain number of nodes on it --> when you arrive at on 
      // of those nodes, look for new opening and follow that path --> do not return to loop
   currentPosition = ((0x00 | next->getYCoor()) << 4) | next->getXCoor();
   return next;
}

bool Maze::hasBetterScore(int dir, int x, int y, float score, MazeNode *next) {
   if (getDirectionNode(x, y, dir)->getScore(7.5, 7.5) < score || 
      (getDirectionNode(x, y, dir)->getScore(7.5, 7.5) == score 
         && getDirectionNode(x, y, dir)->getManhattanDist(7.5, 7.5) < next->getManhattanDist(7.5, 7.5)))
      return true;
}
 
/* Applies A Star algorithm */
void Maze::applyAStarAlgorithm() {
   for (; ;) {
      if (currentPosition == 0x77) { 
         return;
      }
      nextNodeAStar();
   }
}

void Maze::updateMostInner() {
   int lowM = 1000;
   int nx;
   int ny;
   for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
         if (getNode(i, j)->getNumOfTraversals() > 0 && getNode(i, j)->shouldTraverse()) {
            if (getNode(i, j)->getManhattanDist(7.5, 7.5) < lowM) {
               nx = i;
               ny = j;
               lowM = getNode(i, j)->getManhattanDist(7.5, 7.5);
            }
         }
      }
   }
   mostInner = ((0x00 | ny) << 4) | nx;
}

MazeNode *Maze::getMostInnerTraversed() {
   MazeNode *n = getNode(0x000F & mostInner, (0x00F0 & mostInner) >> 4);
   updateMostInner();
   return n;
}

void Maze::applyMazeWalls(bool newwalls[][16][4]) {
   for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
         getNode(i, j)->updateWalls(newwalls[j][i]);
      }
   }
   return;
} 

int Maze::isAdjacent(int x, int y, int a, int b) {
   if (x - a == 1 && y - b == 0)
      return -2;
   if (x - a == -1 && y - b == 0)
      return 2;
   if (x - a == 0 && y - b == 1)
      return -1;
   if (x - a == 0 && y - b == -1) 
      return 1;
   return 0;
}
