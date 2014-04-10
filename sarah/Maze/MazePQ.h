
#ifndef MAZEPRIORITYQUEUE_H
#define MAZEPRIORITYQUEUE_H

#include "MazeNode.h"

class MazePQ {

   private:
		MazeNode **heap;
		int N;
		
		void exch(int a, int b);
		bool nodeGreater(int a, int b);
		void promote(int k);
		void demote(int k);

   public:
		void setValues(int size);
		int getSize();
		void push(MazeNode *add);
		MazeNode *peek();
		MazeNode *pop();
		void freePQ();
		void removeLowest();
		MazeNode *peekLowest();
		float getNodeWeightedScore(MazeNode *n);
};

#endif

