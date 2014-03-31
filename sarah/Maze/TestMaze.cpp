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
   assert(!test1.hasWall(RIGHT));
   assert(!test1.hasWall(DOWN));
   assert(!test1.hasWall(LEFT));
   assert(!test1.hasWall(UP));
   
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
   assert(test1.hasWall(RIGHT));
   assert(test1.hasWall(DOWN));
   assert(test1.hasWall(LEFT));
   assert(test1.hasWall(UP));
   
   w[1] = false;
   w[3] = false;
   MazeNode test2;
   test2.setValues(10, 3, 2, 17, w);
   assert(test2.getXCoor() == 10);
   assert(test2.getYCoor() == 3);
   assert(test2.getScore() == 1.001*2 + 17);
   assert(test2.getManhattanDist() == 2);
   assert(test2.getNumOfTraversals() == 0);
   assert(test2.hasWall(RIGHT));
   assert(!test2.hasWall(DOWN));
   assert(test2.hasWall(LEFT));
   assert(!test2.hasWall(UP));
   
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
         assert((*test3.getNode(i, j)).hasWall(RIGHT) == false);
         assert((*test3.getNode(i, j)).hasWall(LEFT) == false);
         assert((*test3.getNode(i, j)).hasWall(UP) == false);
         assert((*test3.getNode(i, j)).hasWall(DOWN) == false);
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


   /*********** TESTING MAZE ***********/

   printf("\nTESTING MAZE\n");
   Maze test5;
   int c;
   int width;
   int height;
   bool g[16][16][4];

   scanf("%d\n", &width);
   scanf("%d\n", &height);
   
   int hor = 0;
   int ver = 0;
   while ((c = getchar()) != EOF) {

      if (c == '\n') {
         ver++;
         hor = 0;
      }
      else {
         if (ver % 2 == 0) {
            if (ver == 0) {
               if (c == '0') {
                  g[ver/2][hor][3] = false;
               }
               else if (c == '1') {
                  g[ver/2][hor][3] = true;
               }
            }
            else if (ver == height*2) {
               if (c == '0')
                  g[ver/2-1][hor][1] = false;
               else if (c == '1')
                  g[ver/2-1][hor][1] = true;
            }
            else {
               if (c == '0') {
                  g[ver/2][hor][3] = false;
                  g[ver/2-1][hor][1] = false;
               }
               else if (c == '1') {
                  g[ver/2][hor][3] = true;
                  g[ver/2-1][hor][1] = true;
               }
            }

         }
         else {
            if (hor == 0) {
               if (c == '0')
                  g[ver/2][hor][2] = false;
               else if (c == '1')
                  g[ver/2][hor][2] = true;
            }
            else if (hor == height*2) {
               if (c == '0')
                  g[ver/2][hor-1][0] = false;
               else if (c == '1')
                  g[ver/2][hor-1][0] = true;
            }
            else {
               if (c == '0') {
                  g[ver/2][hor][2] = false;
                  g[ver/2][hor-1][0] = false;
               }
               else if (c == '1') {
                  g[ver/2][hor][2] = true;
                  g[ver/2][hor-1][0] = true;
               }
            }
         }

      hor++;  
      }
   }

   test5.setValues(width, height, 0, 15, 7.5, 7.5);

// 7 7
// 66
// Number of nodes traversed: 230
// Number of traversals: 1186

//    88
//    66
//    Number of nodes traversed: 129
// Number of traversals: 200

// 7.5 7.5
//    66
//    Number of nodes traversed: 139
// Number of traversals: 218

// 7 8
//    66
//    Number of nodes traversed: 137
// Number of traversals: 219

// 8 7
//    66
//    Number of nodes traversed: 224
// Number of traversals: 1137

   test5.applyMazeWalls(g, width, height);

   test5.applyAStarAlgorithm();

   int nums = 0;
   printf("\nMazeArray's Number of Traversals\n\n");
   for (int j = 0; j < 16; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test5.getNode(i, j))->hasWall(UP)) a = " -- ";
         else a = "    ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test5.getNode(i, j))->hasWall(LEFT)) a = "|";
         else a = " ";
         printf("%s%2d ", a, (test5.getNode(i, j))->getNumOfTraversals());
         nums += (test5.getNode(i, j))->getNumOfTraversals();

         if (i == 15 && (test5.getNode(i, j))->hasWall(RIGHT))
            printf("|");;
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test5.getNode(i, j))->hasWall(DOWN)) 
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
         if ((test5.getNode(i, j))->hasWall(UP)) a = " -- ";
         else a = "    ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test5.getNode(i, j))->hasWall(LEFT)) a = "|";
         else a = " ";
         printf("%s%2.f ", a, (test5.getNode(i, j))->getManhattanDist());

         if (i == 15 && (test5.getNode(i, j))->hasWall(RIGHT))
            printf("|");;
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test5.getNode(i, j))->hasWall(DOWN)) 
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
         if ((test5.getNode(i, j))->hasWall(UP)) a = " --- ";
         else a = "     ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test5.getNode(i, j))->hasWall(LEFT)) a = "|";
         else a = " ";
         printf("%s%3d ", a, (test5.getNode(i, j))->getStartDist());

         if (i == 15 && (test5.getNode(i, j))->hasWall(RIGHT))
            printf("|");;
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test5.getNode(i, j))->hasWall(DOWN)) 
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
         if ((test5.getNode(i, j))->hasWall(UP)) a = " ----- ";
         else a = "       ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test5.getNode(i, j))->hasWall(LEFT)) a = "|";
         else a = " ";
         printf("%s%5.1f ", a, (test5.getNode(i, j))->getScore());

         if (i == 15 && (test5.getNode(i, j))->hasWall(RIGHT))
            printf("|");;
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test5.getNode(i, j))->hasWall(DOWN)) 
               printf(" ----- ");
            else 
               printf("       ");
         }
         printf("\n");
      }
   }

   printf("\nMazeArray's Solution Nodes\n\n");
   for (int j = 0; j < 16; j++) {
      for (int i = 0; i < 16; i++) {
         const char *a;
         if ((test5.getNode(i, j))->hasWall(UP)) a = " -- ";
         else a = "    ";
         printf("%s", a);
      }
      printf("\n");
      for (int i = 0; i < 16; i++) {
         const char *a, *b;
         if ((test5.getNode(i, j))->hasWall(LEFT)) a = "|";
         else a = " ";
         printf("%s%2d ", a, (test5.getNode(i, j))->checkSolution());

         if (i == 15 && (test5.getNode(i, j))->hasWall(RIGHT))
            printf("|");;
      }
      printf("\n");
      if (j == 15) {
         for (int i = 0; i < 16; i++) {
            const char *a;
            if ((test5.getNode(i, j))->hasWall(DOWN)) 
               printf(" -- ");
            else 
               printf("    ");
         }
         printf("\n");
      }
   }

   printf("Number of nodes traversed: %d\n", test5.getNumOfNodesTraversed());
   printf("Number of traversals: %d\n", nums);
   
   int size5 = 256;
   enum Direction *path5;
   path5 = test5.getAStarSolutionPath(&size5);
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


   // nodes that were not traversed on priority queue
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
