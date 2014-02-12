/*---------------------------------------------------------
astar.h

function and base class for running A* algorithm
---------------------------------------------------------*/

#ifndef astar_h
#define astar_h

// get priority_queue, queue, and stack
using namespace std;

// run the A* algorithm on any implementation of SearchNode
// return the shortest path from start to goal
stack<SearchNode> astar(SearchNode start, SearchNode goal);


/*-------------------------------------------------------*/


// SearchNode is used by astar as elements to search through
class SearchNode {
	public:
		// construct a SearchNode with its parent as parent
		// and identifying itself with neighborID
		// in this case, neighborID is a direction ie [0, 3]
		SearchNode(SearchNode* parent, int neighborID);
		
		// construct a SearchNode with integer coordinates (x, y)
		// and goal node goal
		SearchNode(int x, int y, SearchNode* goal);
	
		// return a queue of all nodes that are "adjacent" to
		// this node
		queue<SearchNode> neighbors();
		
		// the operator used by the priority queue in astar, acts
		// as "<" to satisfy the strict-weak ordering of a pq
		bool operator(const SearchNode& a, const SearchNode& b);
		
		// the inequality operator. check if a and b are not the same.
		// this is defined by the implementation of SearchNode, but
		// can either be an id, coordinates, or memory location
		bool operator!=(const SearchNode& a, const SearchNode& b);
	
		// return a pointer to the parent node (the node that
		// added this one to the priority queue)
		SearchNode* parent();
		
	private:
		SearchNode* parentNode; // parent node
		SearchNode* goalNode; // goal node
		int xValue, yValue; // integer coordinates
};

#endif


/*---------------------------------------------------------

Everything that would be needed if we could use SearchNode
as an interface.  However, c++ does not handle interfaces
nicely, and for space/performance reasons it is better to not
use interfaces / abstract classes. :( 

// run the A* algorithm on any implementation of SearchNode
// return the shortest path from start to goal
stack<SearchNode> astar(SearchNode start, SearchNode goal);

class SearchNode {
	public:
		// return a queue of all nodes that are "adjacent" to
		// this node
		virtual queue<SearchNode> neighbors() = 0;
		
		// the operator used by the priority queue in astar, acts
		// as "<" to satisfy the strict-weak ordering of a pq
		virtual bool operator(const SearchNode& a, const SearchNode& b) = 0;
		
		// the equality operator. check if a and b are the same.
		// this is defined by the implementation of SearchNode, but
		// can either be an id, coordinates, or memory location
		virtual bool operator==(const SearchNode& a, const SearchNode& b) = 0;
	
		// return a pointer to the parent node (the node that
		// added this one to the priority queue)
		virtual SearchNode* parent() = 0;
};

---------------------------------------------------------*/
