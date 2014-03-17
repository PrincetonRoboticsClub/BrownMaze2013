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
static int getArrayIndex(int x, int y, int width) {
   return width * x + y;
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
   mazeArray = (MazeNode **) malloc(6144);
   bool w[4] = {false, false, false, false};

   for (int i = 0; i < lengthX; i++) {
      for (int j = 0; j < lengthY; j++) {
         mazeArray[getArrayIndex(i, j, lengthX)] = (MazeNode *) malloc(sizeof(MazeNode));
         (*mazeArray[getArrayIndex(i, j, lengthX)]).setValues(i, j, abs(i - targetX) + abs(j - targetY), abs(i - startX) + abs(j - startY), w);
      }
   }  

   printf("should be 0 %d\n", (*mazeArray[0]).getYCoor());
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
   
/* Applies A Star algorithm and returns 1 if successful and 0 if unsuccessful */
int Maze::applyAStarAlgorithm() {
   return 0;
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

void Maze::freeMaze() {

}
