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


#include <stdlib.h>
#include "MazeNode.h"
#include <math.h>

// enum Direction {UP = 1, DOWN = -1, RIGHT = 2, LEFT = -2};

void MazeNode::setValues(char inCoords, char inStartDist, char inWalls) {
   coords = inCoords; 
   startDist = inStartDist; 
   walls = inWalls; 
}

char MazeNode::getXCoor() {
   return (0x0F & coords);
}

char MazeNode::getYCoor() {
   return (0xF0 & coords);
}

float MazeNode::getScore(float a, float b) {
   // 4.0001*manhattanDist + 0.9999*startDist + 4.0000*numOfTraversals
   return 4.0001*getManhattanDist(a, b) + 0.9999*((int) startDist) + 4.0000*((int) (0x70 & walls));
}

float MazeNode::getManhattanDist(float a, float b) {
   return abs(((int) getXCoor()) - a) + abs(((int) getYCoor()) - b);
}

char MazeNode::getStartDist() {
   return startDist;
}

char MazeNode::getNumOfTraversals() {
   return (0x07 & (walls >> 4));
}

bool MazeNode::hasWall(int dir) {
   switch (dir) {
      case 2: return (0x01 & walls);
      case -1: return (0x01 & (walls >> 1));
      case -2: return (0x01 & (walls >> 2));
      case 1: return (0x01 & (walls >> 3));
      default: assert(false);
   }
}

// 0 000 ULDR
void MazeNode::updateWalls(char newWalls) {
   newWalls << 4;
   newWalls >> 4; // clearing higher bits

   walls = (walls & 0xF0) || newWalls;
}

void MazeNode::incrementNumOfTraversals() {
   walls += 0x10;
}

void MazeNode::setStartDist(char newStartDist) {
   startDist = newStartDist;
}

char MazeNode::getNumOfOpenWalls() {
   char num = 0;
   for (int i = 0; i < 4; i++) {
      num += (walls >> i) & 0x01
   }
   return num;
}

// STOPPED
bool MazeNode::shouldTraverse() {
   // return (!deadEnd && numOfTraversals < getNumOfOpenWalls());
   return (!isDeadEnd() && ((int) getNumOfTraversals() < (int) getNumOfOpenWalls()))
}

void MazeNode::markDeadEnd() {
   walls = walls | 0x80;
}

bool MazeNode::isDeadEnd() {
   return walls & 0x80;
}
