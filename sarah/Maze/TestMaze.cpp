/******************************************************************************
 * Brown Competition 2014
 * TestMaze.cpp
 *
 * Tests all classes: MazeNode, Maze, and Brain. 
 *
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "MazeNode.h"
#include "Maze.h"
#include <math.h>

int main(void) {
   /******************************* MazeNode *******************************/
   printf("-----------------------------------------------------------\n");
   printf("TESTING MAZENODE\n");
   bool w[4] = {false, false, false, false};
   MazeNode test1;
   test1.setValues(0, 0, 0, 0, w);
   assert(test1.getXCoor() == 0);
   assert(test1.getYCoor() == 0);
   assert(test1.getScore() == 0);
   assert(test1.getManhattanDist() == 0);
   assert(test1.getStartDist() == 0);
   assert(test1.getNumOfTraversals() == 0);
   assert(!test1.rightWall());
   assert(!test1.bottomWall());
   assert(!test1.leftWall());
   assert(!test1.topWall());
   
   test1.incrementNumOfTraversals();
   test1.setStartDist(5);
   test1.incrementNumOfTraversals();
   w[0] = true;
   w[1] = true;
   w[2] = true;
   w[3] = true;
   test1.updateWalls(w);
   
   assert(test1.getScore() == 5);
   assert(test1.getManhattanDist() == 0);
   assert(test1.getStartDist() == 5);
   assert(test1.getNumOfTraversals() == 2);
   assert(test1.rightWall());
   assert(test1.bottomWall());
   assert(test1.leftWall());
   assert(test1.topWall());
   
   w[1] = false;
   w[3] = false;
   MazeNode test2;
   test2.setValues(10, 3, 2, 17, w);
   assert(test2.getXCoor() == 10);
   assert(test2.getYCoor() == 3);
   assert(test2.getScore() == 19);
   assert(test2.getManhattanDist() == 2);
   assert(test2.getStartDist() == 17);
   assert(test2.getNumOfTraversals() == 0);
   assert(test2.rightWall());
   assert(!test2.bottomWall());
   assert(test2.leftWall());
   assert(!test2.topWall());
   
   /********************************* Maze *********************************/
   printf("-----------------------------------------------------------\n");
   printf("TESTING MAZE\n");
   Maze test3;
   test3.setValues(16, 15, 0, 0, 15, 14);
   for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 15; j++) {
         assert((*test3.getNode(i, j)).getXCoor() == i);
         assert((*test3.getNode(i, j)).getYCoor() == j);
         assert((*test3.getNode(i, j)).getManhattanDist() == abs(i - 15) + abs(j - 14));
         assert((*test3.getNode(i, j)).rightWall() == false);
         assert((*test3.getNode(i, j)).leftWall() == false);
         assert((*test3.getNode(i, j)).topWall() == false);
         assert((*test3.getNode(i, j)).bottomWall() == false);
         assert((*test3.getNode(i, j)).getNumOfTraversals() == 0);
         (*test3.getNode(i, j)).incrementNumOfTraversals();
         assert((*test3.getNode(i, j)).getNumOfTraversals() == 1);
         (*test3.getNode(i, j)).incrementNumOfTraversals();
         assert((*test3.getNode(i, j)).getNumOfTraversals() == 2);

         test3.incrementNumOfNodesTraversed();
      }
   }
   assert(test3.getNumOfNodesTraversed() == 16 * 15);
   assert(test3.getLengthX() == 16);
   assert(test3.getLengthY() == 15);
   assert(test3.getSize() == 16 * 15);

   assert((*test3.getStartNode()).getXCoor() == 0);
   assert((*test3.getStartNode()).getYCoor() == 0);
   assert((*test3.getTargetNode()).getXCoor() == 15);
   assert((*test3.getTargetNode()).getYCoor() == 14);
   test3.freeMaze();

   Maze test4;
   test4.setValues(16, 15, 0, 0, 15, 14);

   test4.applyAStarAlgorithm();
   for (int j = 0; j < 15; j++) {
      for (int i = 0; i < 16; i++) {
         printf("| %d ", (test4.getNode(i, j))->getNumOfTraversals());
      }
      printf("|\n");
   }
   test4.freeMaze();

   /******************************** Brain *********************************/
   printf("-----------------------------------------------------------\n");
   printf("TESTING BRAIN\n");
   return 0;

   /* Make a random maze */

   /* Apply a star */

   /* Apply DFS */

   /* Find shortest paths */

   /* Display path visually */

}
