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

/* Represents the walls of the node. Could be represented by an array of 
   booleans but a struct is more expressive option. */
struct NodeWalls {
  bool posX; // right
  bool posY; // down
  bool negX; // left
  bool negY; // up
};

class MazeNode {

  private:
    int xCoor; // horizontal coordinate with zero at left
    int yCoor; // vertical coordinate with zero at top 
    int manhattanDist; // sum of differences of x coordinates and y coordinates from target node
    int startDist; // distance (in number of nodes traversed) from start node
    struct NodeWalls walls; // find out if there are any walls around the node
    int numOfTraversals; // number of times node crossed in finding a sucessful path

  public:
    /* Constructor function takes in coordinates (coorX and coorY), a manhattan distance,
      a start distance, and an array walls of booleans in the order: 
      [positiveX, positiveY, negativeX, negativeY] or [Right, Down, Left, Up].
      
      Right now, all arguments are required though in the future, can write 
      the class so that walls, for example, if NULL will simply be set so that
      no walls exist 

      Old constructor: MazeNode(int coorX, int coorY, int manDist, int startingDist, bool newWalls[]);
    */
    void setValues(int coorX, int coorY, int manDist, int startingDist, bool newWalls[]);

    /* Returns x coordinate */
    int getXCoor();

    /* Returns y coordinate */
    int getYCoor();

    /* Returns node's score (sum of manhattan and start distances) */
    int getScore();

    /* Returns Manhattan distance */
    int getManhattanDist();

    /* Returns distance from starting node */
    int getStartDist();

    /* Returns number of traversals of this node in finding the path */
    int getNumOfTraversals();
    
    /* Returns whether or not there is a right/positive X wall */
    bool rightWall();
    
    /* Returns whether or not there is a bottom/positive Y wall */
    bool bottomWall();

    /* Returns whether or not there is a left/negative X wall */
    bool leftWall();

    /* Returns whether or not there is a top/negative Y wall */
    bool topWall();

    /* Update the walls of the node */
    void updateWalls(bool newWalls[]);

    /* Increments the number of traversals. Cannot directly set the number 
       of traversals because such an action is too dangerous and does not 
       make sense. */
    void incrementNumOfTraversals();

    /* Change distance in nodes from start node. Should not be able to 
       change Manhattan Distance. */
    void setStartDist(int dist);

};

#endif
