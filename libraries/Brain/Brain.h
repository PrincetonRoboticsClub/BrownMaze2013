#ifndef BRAIN_H
#define BRAIN_H

#include "Maze.h"
#include "MazeNode.h"
#include <stdlib.h>
#include <stdio.h>
#include "Robot.h"
#include "WallSensor.h"

class Brain {
	private:
		// bool reset;
		// bool solved;
		Maze maz;
		Robot *r;
		WallSensor *ws;

	public:
		void setValues(Robot *in, WallSensor *wsin); //

		void solveMaze();

		void setReset();

		void travelPath(int *length, int path[]);

		int getRobotDir(int dir);

		void getPath(int fromX, int fromY, int toX, int toY, int *length, int *path); //

		void startFromBeginning();

		void getPathToStart(int *length, int path[], MazeNode* n); //

		void getPathFromStart(int *length, int path[], MazeNode* n); //

		void getPathFromStart(int *length, int path[], MazeNode *nodes[], MazeNode* n); //

		void getWalls();

		void travelSolutionPath();

		Maze *getMaze(); //

};

void doMaze(Robot *r, WallSensor *ws);


int freeRam ();


#endif