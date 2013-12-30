/*---------------------------------------------------------
astar.h

function and base class for running A* algorithm
---------------------------------------------------------*/

stack<SearchNode> astar(SearchNode start, SearchNode goal);


// make this an abstract class?
class SearchNode {
	public:
		SearchNode();
		
		queue<SearchNode> neighbors();
		
		bool operator< (const SearchNode& a, const SearchNode& b);
		bool operator==(const SearchNode& a, const SearchNode& b);
	
	private:
		SearchNode* parent;
};