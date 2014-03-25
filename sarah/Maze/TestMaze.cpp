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
   /*printf("-----------------------------------------------------------\n");
   printf("TESTING MAZENODE\n");
   bool w[4] = {false, false, false, false};
   MazeNode test1;
   test1.setValues(0, 0, 0, 0, w);
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
   
   assert(test1.getScore() == 0);
   assert(test1.getManhattanDist() == 0);
   assert(test1.getNumOfTraversals() == 2);
   assert(test1.hasWall(RIGHT_W));
   assert(test1.hasWall(BELOW_W));
   assert(test1.hasWall(LEFT_W));
   assert(test1.hasWall(ABOVE_W));
   
   w[1] = false;
   w[3] = false;
   MazeNode test2;
   test2.setValues(10, 3, 2, 17, w);
   assert(test2.getXCoor() == 10);
   assert(test2.getYCoor() == 3);
   assert(test2.getScore() == 1.001*2 + 17);
   assert(test2.getManhattanDist() == 2);
   assert(test2.getNumOfTraversals() == 0);
   assert(test2.hasWall(RIGHT_W));
   assert(!test2.hasWall(BELOW_W));
   assert(test2.hasWall(LEFT_W));
   assert(!test2.hasWall(ABOVE_W));
   
   /********************************* Maze *********************************/
   
   /****** TESTING BASIC MAZE OPERATIONS ******/
   /*printf("-----------------------------------------------------------\n");
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

   /*********** TESTING EMPTY MAZE ***********/
   Maze test4;
   test4.setValues(16, 15, 0, 0, 7, 7);
   bool h[] = {1, 0, 1, 1};
   bool l[] = {0, 0, 1, 1};
   test4.getNode(0, 0)->updateWalls(h);
   test4.getNode(1, 0)->updateWalls(l);
   test4.applyAStarAlgorithm();

   printf("\nMazeArray's Number of Traversals\n\n");
   for (int j = 0; j < 15; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(ABOVE_W)) a = " -- ";
         else a = "    ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test4.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         printf("%s%2d ", a, (test4.getNode(i, j))->getNumOfTraversals());

         if (i == 15 && (test4.getNode(i, j))->hasWall(RIGHT_W))
            printf("|");;
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test4.getNode(i, j))->hasWall(BELOW_W)) 
               printf(" -- ");
            else 
               printf("    ");
         }
         printf("\n");
      }
   }

   printf("\nMazeArray's ManhattanDist\n\n");
   for (int j = 0; j < 15; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(ABOVE_W)) a = " -- ";
         else a = "    ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test4.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         printf("%s%2d ", a, (test4.getNode(i, j))->getManhattanDist());
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test4.getNode(i, j))->hasWall(BELOW_W)) 
               printf(" -- ");
            else 
               printf("    ");
         }
         printf("\n");
      }
   }

   printf("\nMazeArray's StartDist\n\n");
   for (int j = 0; j < 15; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(ABOVE_W)) a = " --- ";
         else a = "     ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test4.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         printf("%s%3d ", a, (test4.getNode(i, j))->getStartDist());
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test4.getNode(i, j))->hasWall(BELOW_W)) 
               printf(" --- ");
            else 
               printf("     ");
         }
         printf("\n");
      }
   }

   printf("\nMazeArray's Score\n\n");
   for (int j = 0; j < 15; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(ABOVE_W)) a = " --- ";
         else a = "     ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test4.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         printf("%s%3.f ", a, (test4.getNode(i, j))->getScore());
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test4.getNode(i, j))->hasWall(BELOW_W)) 
               printf(" --- ");
            else 
               printf("     ");
         }
         printf("\n");
      }
   }

   printf("\nMazeArray's Solution Nodes\n\n");
   for (int j = 0; j < 15; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test4.getNode(i, j))->hasWall(ABOVE_W)) a = " -- ";
         else a = "    ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test4.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         printf("%s%2d ", a, (test4.getNode(i, j))->checkSolution());
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test4.getNode(i, j))->hasWall(BELOW_W)) 
               printf(" -- ");
            else 
               printf("    ");
         }
         printf("\n");
      }
   }

   int size = 256;
   enum Adjacent *path;
   path = test4.getPath(&size);
   printf("\n\nSolution path:\n");
   for (int m = 0; m < size; m++) {
      switch (path[m]) {
         case RIGHT: printf("RIGHT\n"); break;
         case LEFT: printf("LEFT\n"); break;
         case DOWN: printf("DOWN\n"); break;
         case UP: printf("UP\n"); break;
      }
   }
   printf("\n");
   test4.freeMaze();

   /*********** TESTING 2013 MAZE ***********/
   printf("\nTESTING 2013 MAZE\n");
   Maze test5;
   test5.setValues(16, 16, 0, 15, 7, 7);

   bool g[16][16][4] = {
      {
         {1,1,0,0},{0,0,1,1},{0,0,0,1},{0,0,0,1},{0,1,0,1},{0,0,0,1},{0,1,0,1},{0,0,0,1},{0,1,0,1},{0,1,0,1},{0,0,0,1},{0,1,0,1},{0,0,0,1},{0,1,0,1},{0,1,0,1},{1,0,0,1}
      },
      {
         {0,0,1,1},{1,0,0,0},{1,0,1,0},{1,0,1,0},{1,1,1,1},{1,0,1,0},{1,1,1,1},{1,0,1,0},{1,0,1,1},{1,0,1,1},{1,0,1,0},{0,0,1,1},{0,1,0,0},{0,1,0,1},{0,1,0,1},{1,0,0,0}
      },
      {
         {1,0,1,0},{1,0,1,0},{1,0,1,0},{1,0,1,0},{1,0,1,1},{1,0,1,0},{1,0,1,1},{1,0,1,0},{1,0,1,0},{1,0,1,0},{1,0,1,0},{0,1,1,0},{0,0,0,1},{0,1,0,1},{0,1,0,1},{1,0,0,0}
      },
      {
         {0,0,1,0},{1,0,0,0},{0,0,1,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0},{0,0,0,0},{0,1,0,0},{0,1,0,1},{0,1,0,0},{0,1,0,1},{0,1,0,1},{1,0,0,0}
      },
      {
         {1,0,1,0},{1,0,1,0},{0,0,1,0},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{1,1,0,1},{1,1,1,0},{1,1,1,0},{0,1,1,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{1,1,0,1},{1,0,1,0}
      },
      {
         {0,0,1,0},{1,0,0,0},{1,0,1,0},{0,0,1,1},{0,1,0,1},{1,0,0,1},{0,1,1,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,0,0,1},{0,1,0,1},{1,0,0,1},{0,0,1,1},{0,0,0,1},{1,0,0,0}
      },
      {
         {1,0,1,0},{1,0,1,0},{1,0,1,0},{0,1,1,0},{1,0,0,1},{1,0,1,0},{0,0,1,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{1,0,0,0},{1,0,1,1},{1,0,1,0},{1,0,1,0},{1,0,1,0},{1,0,1,0}
      },
      {
         {0,0,1,0},{1,0,0,0},{1,0,1,0},{0,0,1,1},{1,1,0,0},{1,0,1,0},{1,0,1,0},{0,0,1,1},{0,0,0,1},{0,1,0,1},{0,1,0,0},{1,0,0,0},{1,0,1,0},{1,0,1,0},{1,0,1,0},{1,0,1,0}
      },
      {
         {1,0,1,0},{1,0,1,0},{1,0,1,0},{0,1,1,0},{1,0,0,1},{0,1,1,0},{1,1,0,0},{0,1,1,0},{1,1,0,0},{0,1,1,1},{0,1,0,1},{1,0,0,0},{0,1,1,0},{0,0,0,0},{1,1,0,0},{1,0,1,0}
      },
      {
         {0,0,1,0},{1,0,0,0},{0,0,1,0},{0,1,0,1},{1,1,0,0},{0,1,1,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,0,0,1},{0,1,0,1},{1,1,0,0},{0,1,1,1},{0,0,0,0},{1,1,0,1},{1,0,1,0}
      },
      {
         {1,0,1,0},{1,0,1,0},{0,0,1,0},{1,1,0,1},{0,0,1,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,0,0,1},{1,1,0,0},{0,0,1,1},{0,1,0,1},{0,1,0,1},{0,1,0,0},{0,1,0,1},{1,1,0,0}
      },
      {
         {0,0,1,0},{1,0,0,0},{0,0,1,0},{1,1,0,1},{1,0,1,0},{0,0,1,1},{0,1,0,1},{0,1,0,1},{0,0,0,0},{1,1,0,1},{1,0,1,0},{0,0,1,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{1,0,0,1}
      },
      {
         {1,0,1,0},{1,0,1,0},{0,0,1,0},{1,1,0,1},{1,0,1,0},{1,0,1,0},{1,1,1,1},{0,0,1,1},{1,1,0,0},{0,1,1,1},{1,0,0,0},{1,0,1,0},{0,0,1,1},{0,1,0,1},{0,1,0,1},{1,1,0,0}
      },
      {
         {1,0,1,0},{1,0,1,0},{0,0,1,0},{1,1,0,1},{1,0,1,0},{1,0,1,0},{0,0,1,1},{1,1,0,0},{0,1,1,1},{0,1,0,1},{1,0,0,0},{1,0,1,0},{1,0,1,0},{0,0,1,1},{0,1,0,1},{1,0,0,1}
      },
      {
         {1,0,1,0},{1,0,1,0},{0,1,1,0},{0,1,0,1},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,0},{1,1,0,0},{0,1,1,0},{1,1,0,0},{0,0,1,1},{1,1,0,0}
      },
      {
         {1,1,1,0},{0,1,1,0},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1},{1,1,0,0},{0,0,1,1}
      },
   };

   test5.applyMazeWalls(g, 16, 16);

   test5.applyAStarAlgorithm(); // seg faulting

   printf("\nMazeArray's Number of Traversals\n\n");
   for (int j = 0; j < 16; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test5.getNode(i, j))->hasWall(ABOVE_W)) a = " -- ";
         else a = "    ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test5.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         printf("%s%2d ", a, (test5.getNode(i, j))->getNumOfTraversals());

         if (i == 15 && (test5.getNode(i, j))->hasWall(RIGHT_W))
            printf("|");;
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test5.getNode(i, j))->hasWall(BELOW_W)) 
               printf(" -- ");
            else 
               printf("    ");
         }
         printf("\n");
      }
   }

   printf("\nMazeArray's ManhattanDist\n\n");
   for (int j = 0; j < 16; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test5.getNode(i, j))->hasWall(ABOVE_W)) a = " -- ";
         else a = "    ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test5.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         printf("%s%2d ", a, (test5.getNode(i, j))->getManhattanDist());
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test5.getNode(i, j))->hasWall(BELOW_W)) 
               printf(" -- ");
            else 
               printf("    ");
         }
         printf("\n");
      }
   }

   printf("\nMazeArray's StartDist\n\n");
   for (int j = 0; j < 16; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test5.getNode(i, j))->hasWall(ABOVE_W)) a = " --- ";
         else a = "     ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test5.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         printf("%s%3d ", a, (test5.getNode(i, j))->getStartDist());
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test5.getNode(i, j))->hasWall(BELOW_W)) 
               printf(" --- ");
            else 
               printf("     ");
         }
         printf("\n");
      }
   }

   printf("\nMazeArray's Score\n\n");
   for (int j = 0; j < 16; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test5.getNode(i, j))->hasWall(ABOVE_W)) a = " --- ";
         else a = "     ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test5.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         printf("%s%3.f ", a, (test5.getNode(i, j))->getScore());
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test5.getNode(i, j))->hasWall(BELOW_W)) 
               printf(" --- ");
            else 
               printf("     ");
         }
         printf("\n");
      }
   }

   printf("\nMazeArray's Solution Nodes\n\n");
   for (int j = 0; j < 16; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test5.getNode(i, j))->hasWall(ABOVE_W)) a = " -- ";
         else a = "    ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test5.getNode(i, j))->hasWall(LEFT_W)) a = "|";
         else a = " ";
         printf("%s%2d ", a, (test5.getNode(i, j))->checkSolution());
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test5.getNode(i, j))->hasWall(BELOW_W)) 
               printf(" -- ");
            else 
               printf("    ");
         }
         printf("\n");
      }
   }

   int size5 = 256;
   enum Adjacent *path5;
   path = test5.getPath(&size5);
   printf("\n\nSolution path:\n");
   for (int m = 0; m < size5; m++) {
      switch (path5[m]) {
         case RIGHT: printf("RIGHT\n"); break;
         case LEFT: printf("LEFT\n"); break;
         case DOWN: printf("DOWN\n"); break;
         case UP: printf("UP\n"); break;
      }
   }
   printf("\n");
   test5.freeMaze();

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
