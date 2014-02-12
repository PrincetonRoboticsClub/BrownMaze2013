#include<search.h>

#include<stack>
#include<queue>


// run the search algorithm on any implementation of SearchNode
// return the "shortest" path from start to goal
stack<SearchNode*> astar(SearchNode* start, SearchNode* goal) {
	// run search to find the shortest path
	priority_queue<SearchNode*, vector<SearchNode*>, SearchNode> pq;
	do {
		queue<SearchNode*> n = start->neighbors();
		while (!n.empty()) {
			pq.push(n.front());
			n.pop;
		}
		start = pq.top();
		pq.pop();
	} while (!start->equals(goal));
	
	// reconstruct the path by following the parents
	stack<SearchNode*> path;
	do {
		path.push(current);
		current = current->parent();
	} while (current != NULL);
	
	return path;
}