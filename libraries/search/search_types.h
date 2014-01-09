
#ifndef search_types_h
#define search_types_h

class AStarNode : public SearchNode {
	private:
		SearchNode* parentNode;
		SearchNode* goalNode;
		int xValue, yValue;
	
	public:
		AStarNode(AStarNode* parent, int neighborID);
		AStarNode(int x, int y, AStarNode* goal);
		
		queue<SearchNode*> neighbors();
		
		int compare(SearchNode* other);
		
		int heuristic();
		
		SearchNode* parent();
};

class DFSNode : public SearchNode {
	private:
		SearchNode* parentNode;
		SearchNode* goalNode;
		int xValue, yValue;
	
	public:
		DFSNode(DFSNode* parent, int neighborID);
		DFSNode(int x, int y, DFSNode* goal);
		
		queue<SearchNode*> neighbors();
		
		int compare(SearchNode* other);
		
		int heuristic();
		
		SearchNode* parent();
};

class BFSNode : public SearchNode {
	private:
		SearchNode* parentNode;
		SearchNode* goalNode;
		int xValue, yValue;
	
	public:
		DFSNode(BFSNode* parent, int neighborID);
		DFSNode(int x, int y, BFSNode* goal);
		
		queue<SearchNode*> neighbors();
		
		int compare(SearchNode* other);
		
		int heuristic();
		
		SearchNode* parent();
};

#endif