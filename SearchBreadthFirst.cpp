// SearchBreadthFirst.cpp
// ===============
//
// Declaration of Search class for Breadth First algorithm
//

#include "SearchBreadthFirst.h"

bool CSearchBreadthFirst::FindPath(TerrainMap& terrain, unique_ptr<SNode> start, unique_ptr<SNode> goal, NodeList& path)
{
	// Rather than finding a real path, my made-up No Star algorithm constructs some nodes and calls them a path.
	NodeList openList;
	NodeList closedList;
	NodeList currentList;
	
	unique_ptr <SNode> temp;

	bool found = false;

	
	while(!found)
	{
		temp.reset(new SNode);

		if (temp->x == && t)
	}

	for (int i = 0; i < 4; i++)
	{
		temp.reset(new SNode);
		temp->x = i;
		temp->y = i * 2;
		path.push_back(move(temp));
	}

	return true;
}