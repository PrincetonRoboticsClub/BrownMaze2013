
AStarNode::AStarNode(AStarNode* parent, int neighborID) {
	parentNode = parent;
	goalNode = parent->goalNode;
	xValue = parent->xValue;
	yValue = parent->yValue;
	// TODO: deal with neighborID
	distanceScore = 0; // TODO: calculate
	movementScore = parent->movementScore; // TODO: calculate
}
AStarNode::AStarNode(int x, int y, AStarNode* goal) {
	xValue = x;
	yValue = y;
	goalNode = goal;
	parentNode = NULL;
	distanceScore = 0; // TODO: calculate
	movementScore = 0;
}

queue<SearchNode*> AStarNode::neighbors() {
	queue<SearchNode*> n;
	for (int nID = 0; nID < 4; nID++) {
		SearchNode* node = new AStarNode(this, nID);
		n.push(node);
	}
	return n;
}

bool AStarNode::compare(SearchNode* other) {
	return xValue == other->xValue 
		&& yValue == other->yValue;
}

int AStarNode::heuristic() {
	return distanceScore + movementScore;
}

SearchNode* AStarNode::parent() {
	return parentNode;
}

//---------------------------------------------------------

DFSNode::DFSNode(DFSNode* parent, int neighborID) {
	parentNode = parent;
	goalNode = parent->goalNode;
	xValue = parent->xValue;
	yValue = parent->yValue;
	// TODO: deal with neighborID
	heuristicScore = 0; // calculate
}
DFSNode::DFSNode(int x, int y, DFSNode* goal) {
	
}

queue<SearchNode*> DFSNode::neighbors() {
	
}

int DFSNode::compare(SearchNode* other) {
	
}

int DFSNode::heuristic() {
	
}

SearchNode* DFSNode::parent() {
	
}

//---------------------------------------------------------

BFSNode::BFSNode(BFSNode* parent, int neighborID) {
	
}
BFSNode::BFSNode(int x, int y, BFSNode* goal) {
	
}

queue<SearchNode*> BFSNode::neighbors() {
	
}

int BFSNode::compare(SearchNode* other) {
	
}

int BFSNode::heuristic() {
	
}

SearchNode* BFSNode::parent() {
	
}