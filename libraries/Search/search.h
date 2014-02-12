/*---------------------------------------------------------
search.h

Heuristic search method and base class for searches.

This is a basic search that searches items in order
of increasing heuristic cost. It uses a priority queue
to do so.

SearchNode can be extended to provide functionality for
A* search, depth first search, and breadth first search by
changing the heuristic() method.
---------------------------------------------------------*/


#ifndef search_h
#define search_h

// get priority_queue and stack
using namespace std;

// run the search algorithm on any implementation of SearchNode
// return the shortest path from start to goal
stack<SearchNode*> search(SearchNode* start, SearchNode* goal);

class SearchNode {
	public:
		// return a queue of all nodes that are "adjacent" to
		// this node
		virtual queue<SearchNode*> neighbors() = 0;
		
		// the operator used by the priority queue in pq, acts
		// as "<" to satisfy the strict-weak ordering of a pq
		bool operator(const SearchNode* a, const SearchNode* b) {
			return a->heuristic() < b->heuristic();
		}
		
		// the equality operator. check if a and b are the same.
		// this is defined by the implementation of SearchNode, but
		// can either be an id, coordinates, or memory location
		virtual int equals(SearchNode* other) = 0;
		
		// return the heuristic value of this node. Used by
		// the class operator to compare nodes
		virtual int heuristic() = 0;
	
		// return a pointer to the parent node (the node that
		// added this one to the priority queue)
		virtual SearchNode* parent() = 0;
};

#endif
