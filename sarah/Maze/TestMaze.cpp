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
   test1.setValues(0, 0, 0, w);
   assert(test1.getXCoor() == 0);
   assert(test1.getYCoor() == 0);
   assert(test1.getScore() == 0);
   assert(test1.getManhattanDist() == 0);
   assert(test1.getNumOfTraversals() == 0);
   assert(!test1.hasWall(RIGHT_W));
   assert(!test1.hasWall(BELOW_W));
   assert(!test1.hasWall(LEFT_W));
   assert(!test1.hasWall(ABOVE_W));
   
   test1.incrementNumOfTraversals();
   test1.incrementNumOfTraversals();
   w[0] = true;
   w[1] = true;
   w[2] = true;
   w[3] = true;
   test1.updateWalls(w);
   
   assert(test1.getScore() == 2);
   assert(test1.getManhattanDist() == 0);
   assert(test1.getNumOfTraversals() == 2);
   assert(test1.hasWall(RIGHT_W));
   assert(test1.hasWall(BELOW_W));
   assert(test1.hasWall(LEFT_W));
   assert(test1.hasWall(ABOVE_W));
   
   w[1] = false;
   w[3] = false;
   MazeNode test2;
   test2.setValues(10, 3, 2, w);
   assert(test2.getXCoor() == 10);
   assert(test2.getYCoor() == 3);
   assert(test2.getScore() == 2);
   assert(test2.getManhattanDist() == 2);
   assert(test2.getNumOfTraversals() == 0);
   assert(test2.hasWall(RIGHT_W));
   assert(!test2.hasWall(BELOW_W));
   assert(test2.hasWall(LEFT_W));
   assert(!test2.hasWall(ABOVE_W));
   
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
         assert((*test3.getNode(i, j)).hasWall(RIGHT_W) == false);
         assert((*test3.getNode(i, j)).hasWall(LEFT_W) == false);
         assert((*test3.getNode(i, j)).hasWall(ABOVE_W) == false);
         assert((*test3.getNode(i, j)).hasWall(BELOW_W) == false);
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

   printf("\nMazeArray's Number of Traversals\n");
   for (int j = 0; j < 15; j++) {
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(ABOVE_W)) a = "  --  ";
         else a = "      ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test4.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         if ((test4.getNode(i, j))->hasWall(RIGHT_W)) b = "|";
         else b = " ";
         printf("%s %2d %s", a, (test4.getNode(i, j))->getNumOfTraversals(), b);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(BELOW_W)) a = "  --  ";
         else a = "      ";
         printf("%s", a);
      }
      printf("\n");
   }

   printf("\nMazeArray's ManhattanDist\n");
   for (int j = 0; j < 15; j++) {
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(ABOVE_W)) a = "  --  ";
         else a = "      ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test4.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         if ((test4.getNode(i, j))->hasWall(RIGHT_W)) b = "|";
         else b = " ";
         printf("%s %2d %s", a, (test4.getNode(i, j))->getManhattanDist(), b);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(BELOW_W)) a = "  --  ";
         else a = "      ";
         printf("%s", a);
      }
      printf("\n");
   }

   printf("\nMazeArray's Score\n");
   for (int j = 0; j < 15; j++) {
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(ABOVE_W)) a = "  --  ";
         else a = "      ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test4.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         if ((test4.getNode(i, j))->hasWall(RIGHT_W)) b = "|";
         else b = " ";
         printf("%s %2d %s", a, (test4.getNode(i, j))->getScore(), b);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(BELOW_W)) a = "  --  ";
         else a = "      ";
         printf("%s", a);
      }
      printf("\n");
   }

   printf("\nMazeArray's Solution Nodes\n");
   for (int j = 0; j < 15; j++) {
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(ABOVE_W)) a = "  --  ";
         else a = "      ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test4.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         if ((test4.getNode(i, j))->hasWall(RIGHT_W)) b = "|";
         else b = " ";
         printf("%s %2d %s", a, (test4.getNode(i, j))->checkSolution(), b);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(BELOW_W)) a = "  --  ";
         else a = "      ";
         printf("%s", a);
      }
      printf("\n");
   }

   int size = 256;
   enum Adjacent path[size];
   test4.findShortestSolutionPath(path, &size, test4.getStartNode(), test4.getTargetNode());
   printf("\n\nSolution path:\n");
   for (int m = 0; m < 35; m++) {
      switch (path[m]) {
         case RIGHT: printf("RIGHT "); break;
         case LEFT: printf("LEFT "); break;
         case BELOW: printf("DOWN "); break;
         case ABOVE: printf("UP "); break;
      }
   }
   printf("\n");
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
