#include<astar.h>

#include<stack>
#include<queue>

// run the A* algorithm on any implementation of SearchNode
// return the shortest path from start to goal
stack<SearchNode> astar(SearchNode start, SearchNode goal) {
	// run A* to find the shortest path
	priority_queue<SearchNode, vector<SearchNode>, SearchNode> pq;
	do {
		queue<SearchNode> n = start.neighbors();
		while (!n.empty()) {
			pq.push(n.front());
			n.pop;
		}
		start = pq.top();
		pq.pop();
	} while (start != goal);
	
	// reconstruct the path by following the parents
	stack<SearchNode> path;
	do {
		path.push(current);
		current = *current.parent();
	} while (current != NULL);
	
	return path;
}

// construct a SearchNode with its parent as parent
// and identifying itself with neighborID
// in this case, neighborID is a direction ie [0, 3]
SearchNode::SearchNode(SearchNode* parent, int neighborID) {
	
}

// construct a SearchNode with integer coordinates (x, y)
// and goal node goal
SearchNode::SearchNode(int x, int y, SearchNode* goal) {
	
}

// return a queue of all nodes that are "adjacent" to
// this node
queue<SearchNode> SearchNode::neighbors() {
	
}

// the operator used by the priority queue in astar, acts
// as "<" to satisfy the strict-weak ordering of a pq
bool SearchNode::operator(const SearchNode& a, const SearchNode& b) {
	
}

// the inequality operator. check if a and b are not the same.
// this is using coordinates to check
bool SearchNode::operator!=(const SearchNode& a, const SearchNode& b) {
	
}

// return a pointer to the parent node (the node that
// added this one to the priority queue)
SearchNode* SearchNode::parent() {
	return parentNode;
}



/*---------------------------------------------------------

Everything that would be needed if we could use SearchNode
as an interface.  However, c++ does not handle interfaces
nicely, and for space/performance reasons it is better to not
use interfaces / abstract classes. :( 

// run the A* algorithm on any implementation of SearchNode
// return the shortest path from start to goal
stack<SearchNode> astar(SearchNode start, SearchNode goal) {
	// run A* to find the shortest path
	priority_queue<SearchNode, vector<SearchNode>, SearchNode> pq;
	do {
		queue<SearchNode> n = start.neighbors();
		while (!n.empty()) {
			pq.push(n.front());
			n.pop;
		}
		start = pq.top();
		pq.pop();
	} while (start != goal);
	
	// reconstruct the path by following the parents
	stack<SearchNode> path;
	do {
		path.push(current);
		current = *current.parent();
	} while (current != NULL);
	
	return path;
}

---------------------------------------------------------*/