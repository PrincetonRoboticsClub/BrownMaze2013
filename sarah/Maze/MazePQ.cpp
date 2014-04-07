// parents at k/2
// children at 2k and 2k + 1

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "MazePQ.h"
#include "Maze.h"


void MazePQ::setValues(int size) {
	heap = (MazeNode **) malloc(sizeof(MazeNode*)*(size + 1));
	N = 0;
}

void MazePQ::freePQ() {
	free(heap);
}

void MazePQ::exch(int a, int b) {
	MazeNode *temp = heap[a];
	heap[a] = heap[b];
	heap[b] = temp;
}

bool MazePQ::nodeGreater(int a, int b) {
	// test if a is greater than b
	// if (heap[a]->getScore(7.5, 7.5) > heap[b]->getScore(7.5, 7.5))
	// 	return true;
	// else if (heap[a]->getScore(7.5, 7.5) == heap[b]->getScore(7.5, 7.5) 
	// 	&& heap[a]->getManhattanDist(7.5, 7.5) > heap[b]->getManhattanDist(7.5, 7.5))
	// 	return true;

	// SARAH make it so that the heap returns the one with the start distance closest to the reference node (the node currently being worked on)
	if (heap[a]->getScore(7.5, 7.5) > heap[b]->getScore(7.5, 7.5))
		return true;
	else if (heap[a]->getScore(7.5, 7.5) == heap[b]->getScore(7.5, 7.5) 
		&& heap[a]->getManhattanDist(7.5, 7.5) > heap[b]->getManhattanDist(7.5, 7.5))
		return true;

	// if all tied or less than, returns false
	return false;
}

void MazePQ::promote(int k) {
	while (k > 1 && nodeGreater(k/2, k)) {
		exch(k, k/2);
		k = k/2;
	}
}

void MazePQ::demote(int k) {
	while (2*k <= N) {
		int j = 2*k;
		if (j < N && nodeGreater(j, j + 1))
			j++;
		if (!nodeGreater(k, j))
			break;
		exch(k, j);
		k = j;
	}
}


void MazePQ::push(MazeNode *add) {
	N++;
	heap[N] = add;
	promote(N);
}

MazeNode *MazePQ::peek() {
	return heap[1];
}

MazeNode *MazePQ::pop() {
	MazeNode *pop = heap[1];
	exch(1, N--);
	demote(1);
	return pop;
}

int MazePQ::getSize() {
	return N;
}


