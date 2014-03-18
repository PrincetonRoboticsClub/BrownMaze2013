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

enum Adjacent {ABOVE, BELOW, RIGHT, LEFT};

class Maze {

  private:
   int lengthX; // width (horizontal length) of maze
   int lengthY; // length (vertical length) of maze
   int numTraversed; // number of nodes traversed 
   bool pathFound; // whether or not a successful path has been found

   MazeNode** mazeArray; // an array of MazeNodes to represent rectangular maze

   MazeNode *nodeStart; // pointer to MazeNode in mazeArray of start node
   MazeNode *nodeTarget; // pointer to MazeNode in mazeArray of start node
   MazeNode *currentPosition; // pointer to MazeNode in mazeArray of start node

  public:
   /* Maze constructor takes dimensions of a rectangular maze represented
      by an array of MazeNodes, and the coordinates of the start node and target 
      node. The nodes are ordered so that the origin (0,0) node is in the 
      top left. 

      Old constructor: Maze(int lenX, int lenY, int startX, int startY, int targetX, int targetY);
   */
   void setValues(int lenX, int lenY, int startX, int startY, int targetX, int targetY);

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

   /* Returns pointer node above (lower Y coordinate) specified coordinates */
   MazeNode *getNodeAbove(int x, int y);
   
   /* Returns pointer node below (higher Y coordinate)  specified coordinates */
   MazeNode *getNodeBelow(int x, int y);

   /* Returns pointer node to the right (higher X coordinate) of specified coordinates */
   MazeNode *getNodeRight(int x, int y);

   /* Returns pointer node to the left (lower X coordinate) of specified coordinates */
   MazeNode *getNodeLeft(int x, int y);

   bool edgeOfArray(int x, int y, enum Adjacent dir);

   /* Increments number of Nodes traversed */
   void incrementNumOfNodesTraversed();
   
   /* Applies A Star algorithm and returns 1 if successful and 0 if unsuccessful */
   int applyAStarAlgorithm();

   /* Applies Tremaux algorithm and returns 1 if successful and 0 if unsuccessful */
   int applyTremauxAlgorithm();

   /* Only use after applying an algorithm. Finds the shortest path and 
      returns 1 if successful and 0 if unsuccessful */
   int findShortestSolutionPath();

   void changeCurrentNode(int x, int y);

   void freeMaze();
};

#endif