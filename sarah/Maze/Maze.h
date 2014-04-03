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

#include "MazePQ.h"
#include "MazeNode.h"
#include <vector>

class Maze {

  private:
   int lengthX; // width (horizontal length) of maze
   int lengthY; // length (vertical length) of maze
   int numTraversed; // number of nodes traversed 
   bool solutionFound; // whether or not a successful path has been found
   enum Direction *path;
   int pathLength;

   MazeNode** mazeArray; // an array of MazeNodes to represent rectangular maze

   MazeNode *nodeStart; // pointer to MazeNode in mazeArray of start node
   MazeNode *nodeTarget; // pointer to MazeNode in mazeArray of start node
   MazeNode *currentPosition; // pointer to MazeNode in mazeArray of start node
   MazePQ q;

  public:
   /* Maze constructor takes dimensions of a rectangular maze represented
      by an array of MazeNodes, and the coordinates of the start node and target 
      node. The nodes are ordered so that the origin (0,0) node is in the 
      top left. 

      Old constructor: Maze(int lenX, int lenY, int startX, int startY, int targetX, int targetY);
   */
   void setValues(int lenX, int lenY, int startX, int startY, double targetX, double targetY);

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

   /* Returns pointer adjacent node as indicated by dir to the specified coordinates */
   MazeNode *getDirectionNode(int x, int y, enum Direction dir);

   bool canTravel(int x, int y, enum Direction dir);

   /* Increments number of Nodes traversed */
   void incrementNumOfNodesTraversed();

   /* Changes position of current node */
   void changeCurrentNode(int x, int y);

   /* Frees memory of nodes */
   void freeMaze();

   MazeNode *nextNodeAStar();

   enum Direction *getAStarSolutionPath(int *length);

   /* Applies A Star algorithm */
   void applyAStarAlgorithm();

   void applyMazeWalls(bool newwalls[16][16][4], int width, int height);

   void updateStartDistances(int x, int y);

   bool hasBetterScore(enum Direction dir, int x, int y, int score, MazeNode *next);
};

#endif
