/******************************************************************************
 * Brown Competition 2014
 * MazeNode.h
 *
 * Models a block (or node) in a rectangular maze. Each block has coordinates,
 * walls, a number to keep track of the number of traversals across the block
 * that occur in finding a path to the target node, and a score that is used
 * to figure out whether or not someone attempting to solve the maze should
 * go forward, back, left or right from the current node. Will ultimately be
 * used by the classes Maze.cpp and Brain.cpp. Use TestMaze.cpp to perform
 * debugging or to add tests.
 *
 * This class is made so that each node is its own independent unit and is not
 * linked to the nodes beside it. That representation is fulfilled in the 
 * Maze class.
 *
******************************************************************************/


// when f's are equal, compare h values
// http://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "MazeNode.h"

/* Constructor function takes in coordinates (coorX and coorY), a manhattan distance,
   and an array walls of booleans in the order: 
   [positiveX, positiveY, negativeX, negativeY] or [Right, Down, Left, Up].

   Right now, all arguments are required though in the future, can write 
   the class so that walls, for example, if NULL will simply be set so that
   no walls exist */
void MazeNode::setValues(int coorX, int coorY, int manDist, int startingDist, bool newWalls[]) {
   assert(coorX >= 0);
   assert(coorY >= 0);
   assert(manDist >= 0);

   xCoor = coorX; // horizontal coordinate with zero at left
   yCoor = coorY; // vertical coordinate with zero at top 
   manhattanDist = manDist; // sum of differences of x coordinates and y coordinates from target node
   startDist = startingDist; // shortest distance in number of nodes from start node

   walls.posX = newWalls[0];
   walls.posY = newWalls[1];
   walls.negX = newWalls[2];
   walls.negY = newWalls[3];

   numOfTraversals = 0; // number of times node crossed in finding a sucessful path
   solutionNode = false; // true if part of solution path, false otherwise
}

/* Returns x coordinate */
int MazeNode::getXCoor() {
   return xCoor;
}

/* Returns y coordinate */
int MazeNode::getYCoor() {
   return yCoor;
}

/* Returns node's score (sum of manhattan and number of traversals) */
double MazeNode::getScore() {
   return manhattanDist+ startDist + 4.000001*numOfTraversals;
   // 4.001 to get there in fastest time
   // 2.001 to get there in reasonable time with reasonable short path
   // 1 to get there with shortest path
}

/* Returns Manhattan distance */
int MazeNode::getManhattanDist() {
   return manhattanDist;
}

/* Returns starting distance */
int MazeNode::getStartDist() {
   return startDist;
}

/* Returns number of traversals of this node in finding the path */
int MazeNode::getNumOfTraversals() {
   return numOfTraversals;
}

/* Returns whether or not there is a right/positive X wall */
bool MazeNode::hasWall(enum Direction dir) {
   switch (dir) {
      case RIGHT: return walls.posX;
      case LEFT: return walls.negX;
      case DOWN: return walls.posY;
      case UP: return walls.negY;
      default: assert(false);
   }
   return walls.posX;
}

/* Update the walls of the node */
void MazeNode::updateWalls(bool newWalls[]) {
   walls.posX = newWalls[0];
   walls.posY = newWalls[1];
   walls.negX = newWalls[2];
   walls.negY = newWalls[3];
}

/* Increments the number of traversals. Cannot directly set the number 
   of traversals because such an action is too dangerous and does not 
   make sense. */
void MazeNode::incrementNumOfTraversals() {
   numOfTraversals++;
}

/* Marks the node as a part of the solution path */
void MazeNode::markSolution() {
   solutionNode = true;
}

/* Unmarks the node as a part of the solution path */
void MazeNode::unmarkSolution() {
   solutionNode = false;
}

/* Changes start distcance */
void MazeNode::setStartDist(int dist) {
   startDist = dist;
}

/* Returns true if node is part of solution, false otherwise */
bool MazeNode::checkSolution() {
   return solutionNode;
}

