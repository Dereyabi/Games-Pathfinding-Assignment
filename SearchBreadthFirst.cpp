// SearchBreadthFirst.cpp
// ===============
//
// Declaration of Search class for Breadth First algorithm
//

#include "AlgorithmSearch.h"

bool CSearchBreadthFirst::FindPath(TerrainMap& terrain, unique_ptr<SNode>& start, unique_ptr<SNode>& goal, NodeList& path, int mapXLength, int mapYLength, I3DEngine* myEngine, vector<vector<IModel*>>& modelMap)
{
	NodeList openList;
	NodeList closedList;
	NodeList currentList;
	
	vector<SCurrentNode> currentNodePosition = { { 0, 1}, {1, 0 }, { 0, -1 }, { -1, 0 } };		//vector of am x and y coordinate, used in checking north, east, south and west

	
	unique_ptr <SNode> temp;
	unique_ptr <SNode> currentNode;

	if (terrain[start->y][start->x] == Wall || terrain[goal->y][goal->x] == Wall)				//initial check to see if either the start or end is gon the goal
	{
		cout << "the start or end is on a wall" << endl;
		return false;
	}

	openList.push_back(move(start));			//puts starting coordinates on the open list

	bool found = false;							//stops the while loop if the goal is found
	
	

	while (!openList.empty() || !found)			//loops until the goal is found or until the openlist is empty
	{
		if (openList.empty())					//second openlist empty check, wasnt working without it for some reason
		{
			break;
		}
		else
		{
			currentNode = move(openList.front());						//moves first on openlist to currentNode 
			openList.pop_front();										//pops it from the openlist
																		
			if (currentNode->y == goal->y && currentNode->x == goal->x)	//checks whether the current node is the goal, if true move the goal node onto the closed list for outputting and break
			{
				closedList.push_back(move(currentNode));
				found = true;
				break;
			}
			else											//if not use an iterator to loop through the N(orth), E, S, W checker, if the node it looks at isnt 
			{												//on the openlist or closed list and isnt a wall or the out of bounds of the vector it puts it on the open list

				for (auto it = currentNodePosition.begin(); it != currentNodePosition.end(); ++it)
				{
					if (!ListSearch(openList, currentNode.get(), (*it)) && (!ListSearch(closedList, currentNode.get(), (*it)))
						&& BoundrySearch(terrain, currentNode.get(), (*it), mapXLength, mapYLength) != Wall)					//wall check needs + or - on current node depending on direction 
					{
						temp.reset(new SNode);
						temp->y = currentNode->y + (*it).y;
						temp->x = currentNode->x + (*it).x;
						temp->parent = currentNode.get();
						openList.push_back(move(temp));
					}
				}
				cout << currentNode->x << ", " << currentNode->y << endl;		//outputs current node to console to debug the path that its making
				modelMap[currentNode->y][currentNode->x]->SetSkin("checked1.jpg");
				closedList.push_back(move(currentNode));						//finaly puts the current node thats checked all nodes round it on the closed list 
			}
		}
	}

	//if (found == true)
	//{
	//	vector<SNode*> parents;
	//	parents.push_back(currentNode->parent);

	//	bool pathConstructed = false;

	//	while (pathConstructed == false)
	//	{
	//		if (parents.back() = NULL)
	//		{

	//		}
	//	}
	//}


	for (auto it = closedList.begin(); it != closedList.end(); ++it)		//outputs all nodes that were checked and put on the closed list to debug final path 
	{
		cout << (*it)->x << " " << (*it)->y << endl;
	}

	if (found == true)		//returns whether or not the path was found
	{
		return true;
	}
	
	return false;
	
}

bool CSearchBreadthFirst::ListSearch( NodeList& currentList, SNode* currentNode, SCurrentNode& currentPosition)		//searches N, E, S, W to see if any of the nodes are on
{																													//the list that is provided 
	for (auto it = currentList.begin(); it != currentList.end(); ++it)
	{
		if ((*it)->x == currentNode->x + currentPosition.x && (*it)->y == currentNode->y + currentPosition.y)
		{
			return true;
		}
	}
	return false;
}

ETerrainCost CSearchBreadthFirst::BoundrySearch(TerrainMap& terrain, SNode* currentNode, SCurrentNode& currentPosition, int mapXLength, int mapYLength)
{
	if ( currentNode->x + currentPosition.x >= mapXLength  || currentNode->y + currentPosition.y >= mapYLength || currentNode->x + currentPosition.x < 0 || currentNode->y + currentPosition.y < 0)	//checks if the node searched is a block or out of bounds
	{
		cout << "its a wall" << endl;					//returns as if it was a wall if it is detected to be out of bounds of the vector
		return Wall;
	}
	cout << "not a wall" << endl;
	return terrain[currentNode->y + currentPosition.y][currentNode->x + currentPosition.x];
}
