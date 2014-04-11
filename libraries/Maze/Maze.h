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

#ifndef MAZE_H
#define MAZE_H

#include "MazeNode.h"
#include <vector>

class Maze {

  private:
   // assume 16 by 16 maze
   MazeNode mazeArray[256]; // should only be a pointer (4 bytes) plus 256*4 bytes for all the nodes
   char nodeStart; // x coord is lower 4 bits, y coord is upper 4 bits 
   char currentPosition; // x coord is lower 4 bits, y coord is upper 4 bits 

   char numTraversed; // only necessary for testing, 8 bits
   bool solutionFound; // whether or not a successful path has been found, only necessary for testing
   char mostInner;

  public:
   void setValues(char startX, char startY);

   /* Returns the number of nodes traversed */
   int getNumOfNodesTraversed();

   /* Return the horizontal length (X dimension) of the maze */
   int getLengthX();

   /* Return the vertical length (Y dimension) of the maze */
   int getLengthY();
   
   /* Return size of (number of nodes in) the array */
   int getSize();

   /* Returns pointer to current MazeNode */
   MazeNode *getCurrentNode();
   
   /* Returns pointer to start MazeNode */
   MazeNode *getStartNode();

   /* Returns pointer to target MazeNode */
   MazeNode *getTargetNode();

   /* Returns pointer node with specified coordinates */
   MazeNode *getNode(int x, int y);

   /* Returns pointer adjacent node as indicated by dir to the specified coordinates 
      directions: {UP = 1, DOWN = -1, RIGHT = 2, LEFT = -2}; */
   MazeNode *getDirectionNode(int x, int y, int dir);

   bool canTravel(int x, int y, int dir);

   /* Increments number of Nodes traversed */
   void incrementNumOfNodesTraversed();

   /* Changes position of current node */
   void changeCurrentNode(char x, char y);

   MazeNode *nextNodeAStar();

   void getAStarSolutionPath(int path[256], int *length);

   /* Applies A Star algorithm */
   void applyAStarAlgorithm();

   void applyMazeWalls(bool newwalls[16][16][4]);

   void updateStartDistances(int x, int y);

   MazeNode *getMostInnerTraversed();

   void updateMostInner();

   /* directions: {UP = 1, DOWN = -1, RIGHT = 2, LEFT = -2}; */
   bool hasBetterScore(int dir, int x, int y, float score, MazeNode *next);

   int isAdjacent(int x, int y, int a, int b);

};

#endif
