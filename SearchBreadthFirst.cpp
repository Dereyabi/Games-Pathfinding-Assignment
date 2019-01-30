// SearchBreadthFirst.cpp
// ===============
//
// Declaration of Search class for Breadth First algorithm
//

#include "SearchBreadthFirst.h"

bool ListSearch(NodeList& currentList, int x, int y);

bool CSearchBreadthFirst::FindPath(TerrainMap& terrain, unique_ptr<SNode>& start, unique_ptr<SNode>& goal, NodeList& path)
{
	NodeList openList;
	NodeList closedList;
	NodeList currentList;
	
	unique_ptr <SNode> temp;
	unique_ptr <SNode> currentNode;

	

	openList.push_back(move(start));

	bool found = false;
	

	while(!openList.empty() && found == false)
	{

		currentNode = move(openList.front());
		openList.pop_front();

		if (currentNode->x == goal->y && currentNode->y == goal->x)
		{
			found = true;
		}


		//North
		if (!ListSearch(openList, currentNode->x + 1, currentNode->y) && !ListSearch(closedList, currentNode->x + 1, currentNode->y) && terrain[currentNode->x + 1][currentNode->y] != Wall)					//wall check needs + or - on current node depending on direction 
		{
			temp.reset(new SNode);
			temp->x = currentNode->x + 1;
			temp->y = currentNode->y;
			cout << currentNode->y << ", " << currentNode->x << endl;
			openList.push_back(move(temp));
		}

		//East
		if (!ListSearch(openList, currentNode->x, currentNode->y + 1) && !ListSearch(closedList, currentNode->x, currentNode->y + 1) && terrain[currentNode->x][currentNode->y + 1] != Wall)					//wall check needs + or - on current node depending on direction 
		{
			temp.reset(new SNode);
			temp->x = currentNode->x;
			temp->y = currentNode->y + 1;
			cout << currentNode->y << ", " << currentNode->x << endl;
			openList.push_back(move(temp));
		}
		
		//South
		if (!ListSearch(openList, currentNode->x - 1, currentNode->y) && !ListSearch(closedList, currentNode->x - 1, currentNode->y) && terrain[currentNode->x - 1][currentNode->y] != Wall)					//wall check needs + or - on current node depending on direction 
		{
			temp.reset(new SNode);
			temp->x = currentNode->x - 1;
			temp->y = currentNode->y;
			cout << currentNode->y << ", " << currentNode->x << endl;
			openList.push_back(move(temp));
		}

		//West
		if (!ListSearch(openList, currentNode->x, currentNode->y - 1) && !ListSearch(closedList, currentNode->x, currentNode->y - 1) && terrain[currentNode->x][currentNode->y - 1] != Wall)					//wall check needs + or - on current node depending on direction 
		{
			temp.reset(new SNode);
			temp->x = currentNode->x;
			temp->y = currentNode->y - 1;
			cout << currentNode->y << ", " << currentNode->x << endl;
			openList.push_back(move(temp));
		}

		

		closedList.push_back(move(currentNode));
	}


	for (auto it = closedList.begin(); it != closedList.end(); ++it)
	{
		cout << (*it)->x << " " << (*it)->y << endl;
	}

	return true;
}

bool ListSearch( NodeList& currentList, int x, int y)					//sent the open list, then the current coordinates + or - 1 on either x or y depending on direction
{
	bool found = false;													//


	for (auto it = currentList.begin(); it != currentList.end(); ++it)
	{
		if ((*it)->x == x && (*it)->y == y)
		{
			cout << "Item found in the list" << endl;
			return true;
		}
	}

	if (found == false)
	{
		cout << "Item not in the list" << endl;
		return false;
	}

	return false;
}