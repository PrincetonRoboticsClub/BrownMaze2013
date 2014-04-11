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

#ifndef MAZENODE_H
#define MAZENODE_H

#include "stdint.h"

// enum Direction {UP = 1, DOWN = -1, RIGHT = 2, LEFT = -2};

class MazeNode {

  private:
    char coords; // horizontal coordinate (low 4 bits), vertical coordinate (high 4 bits)
    char startDist; // shortest distance in number of nodes from start node, 8 bits
    char walls; // walls (low 4 bits), number of traversals (middle 3 bits), whether or not it's a dead end (high 1 bit)
                // walls: right (bit 0), down (bit 1), left (bit 2), up (bit 3)

  public:
    void setValues(char inX, char inY, char inStartDist, char inWalls);

    /* Returns x coordinate */
    int getXCoor();

    /* Returns y coordinate */
    int getYCoor();

    /* Returns node's score (sum of manhattan and number of traversals) */
    float getScore(float a, float b);

    /* Returns Manhattan distance */
    float getManhattanDist(float a, float b);

    /* Returns start distance */
    int getStartDist();

    /* Returns number of traversals of this node in finding the path */
    int getNumOfTraversals();
    
    /* Returns whether or not there is a wall in the indicated direction */
    bool hasWall(int dir);

    /* Walls should be low four bits */
    void updateWalls(bool newW[4]);

    /* Increments the number of traversals. Cannot directly set the number 
       of traversals because such an action is too dangerous and does not 
       make sense. */
    void incrementNumOfTraversals();

    /* Changes start distcance */
    void setStartDist(char newStartDist);

    int getNumOfOpenWalls();

    bool shouldTraverse();

    void markDeadEnd();

    bool isDeadEnd();
};

#endif
