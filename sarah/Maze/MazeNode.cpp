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
#include <stdio.h>
#include "MazeNode.h"
#include <math.h>

// enum Direction {UP = 1, DOWN = -1, RIGHT = 2, LEFT = -2};

void MazeNode::setValues(char inX, char inY, char inStartDist, char inWalls) {
   coords = ((0x00 | inY) << 4) | inX; 
   startDist = inStartDist;
   walls = 0x0F & inWalls; 
}

int MazeNode::getXCoor() {
   return (0x000F & coords);
}

int MazeNode::getYCoor() {
   return ((0x00F0 & coords) >> 4);
}

float MazeNode::getScore(float a, float b) {
   // 4.0001*manhattanDist + 0.9999*startDist + 4.0000*numOfTraversals
   return 4.0001*getManhattanDist(a, b) + 0.9999*(getStartDist()) + 4.0001*(getNumOfTraversals());
}

float MazeNode::getManhattanDist(float a, float b) {
   return fabs((float) getXCoor() - a) + fabs((float) getYCoor() - b);
}

int MazeNode::getStartDist() {
   return 0x00FF & startDist;
}

int MazeNode::getNumOfTraversals() {
   return (0x0007 & (walls >> 4));
}

bool MazeNode::hasWall(int dir) {
   switch (dir) {
      case 2: return (0x01 & walls);
      case -1: return (0x01 & (walls >> 1));
      case -2: return (0x01 & (walls >> 2));
      case 1: return (0x01 & (walls >> 3));
   }
}

// 0 000 ULDR
void MazeNode::updateWalls(bool newW[4]) {
   char newWalls = 0x00;
   for (int i = 3; i >= 0; i--) {
      if (newW[i]) 
         newWalls = newWalls | 0x01;
      newWalls = newWalls << 1;
   }
   walls = (walls & 0xF0) | (newWalls >> 1);
}

void MazeNode::incrementNumOfTraversals() {
   walls += 0x10;
}

void MazeNode::setStartDist(char newStartDist) {
   startDist = newStartDist;
}

int MazeNode::getNumOfOpenWalls() {
   int num = 4;
   for (int i = 0; i < 4; i++) {
      if ((walls >> i) & 0x01) {
         num--;
      }
   }
   return num;
}

bool MazeNode::shouldTraverse() {
   // return (!deadEnd && numOfTraversals < getNumOfOpenWalls());
   return (!isDeadEnd() && (getNumOfTraversals() < getNumOfOpenWalls()));
}

void MazeNode::markDeadEnd() {
   walls = walls | 0x80;
}

bool MazeNode::isDeadEnd() {
   return walls & 0x80;
}
