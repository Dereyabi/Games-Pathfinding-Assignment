//Jay Stewart
// SearchAStar.cpp
// ===============
//
// Declaration of Search class for Breadth First algorithm
//

#include "AlgorithmSearch.h"

bool CSearchAStar::FindPath(TerrainMap& terrain, unique_ptr<SNode>& start, unique_ptr<SNode>& goal, NodeList& path, int mapXLength, int mapYLength, I3DEngine* myEngine, vector<vector<IModel*>>& modelMap)
{
	path.clear();
	NodeList openList;
	NodeList closedList;
	NodeList currentList;

	vector<SCurrentNode> currentNodePosition = { { 0, 1 },{ 1, 0 },{ 0, -1 },{ -1, 0 } };		//vector of am x and y coordinate, used in checking north, east, south and west


	unique_ptr <SNode> temp;
	unique_ptr <SNode> currentNode;

	int cost;

	if (terrain[start->y][start->x] == Wall || terrain[goal->y][goal->x] == Wall)				//initial check to see if either the start or end is on the goal
	{
		cout << "the start or end is on a wall" << endl;
		return false;
	}

	start->score = ManhattanDistance(start.get(), goal);
	openList.push_back(move(start));			//push initial coordinates onto openlist 
	

	bool found = false;
	bool newNodeCreated = false;
	int sortCount = 0;

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
						//reset node to a new one
						temp.reset(new SNode);
						//set the position of the new node to the old one + or - in whatever axis is being checked 
						temp->y = currentNode->y + (*it).y;
						temp->x = currentNode->x + (*it).x;
						//set the new node score to the old one then add on score from terrain and adjust for manhattan distence 
						//newcost sends over the map, the temp node and the current node cost 
						//gets the cost of the tile its going on then adds it to the current node cost
						temp->terrainCost = ScoreCheck(terrain, temp.get()) + currentNode->terrainCost;
						int heuristic = ManhattanDistance(temp.get(), goal);
						temp->score = temp->terrainCost + heuristic;
					
						//set the parent of the new node to the old node 
						temp->parent = currentNode.get();
						//push the new node onto the openList
						openList.push_back(move(temp));
						newNodeCreated = true;
					}
					else if (ListSearch(closedList, currentNode.get(), (*it)))
					{
						temp.reset(new SNode);
						temp->y = currentNode->y + (*it).y;
						temp->x = currentNode->x + (*it).x;

						temp->terrainCost = ScoreCheck(terrain, temp.get()) + currentNode->terrainCost;
						int heuristic = ManhattanDistance(temp.get(), goal);
						temp->score = temp->terrainCost + heuristic;

						if (currentNode->score > temp->score)
						{
							openList.erase;  //this needs to delete the old snode on the openlist and replace it with the new one with a lower score
							//set the parent of the new node to the old node 
							temp->parent = currentNode.get();
							//push the new node onto the openList
							openList.push_back(move(temp));
							newNodeCreated = true;
						}
						
					}

					
				}

				cout << currentNode->x << ", " << currentNode->y << "Score: " << currentNode->score << endl;		//outputs current node to console to debug the path that its making
				modelMap[currentNode->y][currentNode->x]->SetSkin("checked1.jpg");
				cout << "sort count is: " << sortCount << endl;
				closedList.push_back(move(currentNode));						//finaly puts the current node thats checked all nodes round it on the closed list 

				//sort the open list by score 
				if (newNodeCreated)
				{
					sort(openList.begin(), openList.end(),
						[](unique_ptr<SNode>& lhs, unique_ptr<SNode>& rhs)
					{
						return lhs->score < rhs->score;
					});

					sortCount++;
					newNodeCreated = false;
				}
			}
		}
	}

	if (found == true)
	{
		SNode* parentNode = closedList.back().get();
		while (parentNode != NULL)
		{
			auto it = closedList.begin();
			while (it != closedList.end())
			{
				if ((*it)->x == parentNode->x && (*it)->y == parentNode->y)
				{
					temp = move((*it));
					path.push_front(move(temp));
					closedList.erase(it);
					break;
				}
				it++;
			}
			parentNode = parentNode->parent;
		}
	}

	if (found == true)		//returns whether or not the path was found
	{
		return true;
	}

	return false;

}

bool CSearchAStar::ListSearch(NodeList& currentList, SNode* currentNode, SCurrentNode& currentPosition)
{
	for (auto it = currentList.begin(); it != currentList.end(); ++it)
	{
		if ((*it)->x == currentNode->x + currentPosition.x && (*it)->y == currentNode->y + currentPosition.y)
		{
			return true;
		}
	}
	return false;
}

ETerrainCost CSearchAStar::BoundrySearch(TerrainMap& terrain, SNode* currentNode, SCurrentNode& currentPosition, int mapXLength, int mapYLength)
{
	if (currentNode->x + currentPosition.x >= mapXLength || currentNode->y + currentPosition.y >= mapYLength || currentNode->x + currentPosition.x < 0 || currentNode->y + currentPosition.y < 0)	//checks if the node searched is a block or out of bounds
	{
		cout << "its a wall" << endl;					//returns as if it was a wall if it is detected to be out of bounds of the vector
		return Wall;
	}
	cout << "not a wall" << endl;
	return terrain[currentNode->y + currentPosition.y][currentNode->x + currentPosition.x];
}

int CSearchAStar::ScoreCheck(TerrainMap& terrain, SNode* currentNode)
{
	vector<ETerrainCost> row;
	row = terrain[currentNode->y];
	if (row[currentNode->x] == Clear)
	{
		return 1;
	}
	if (row[currentNode->x] == Wood)
	{
		return 2;
	}
	if (row[currentNode->x] == Water)
	{
		return 3;
	}

}

int CSearchAStar::ManhattanDistance(SNode* currentNode, unique_ptr<SNode>& goal)
{
	int distance;

	distance = abs(currentNode->x - goal->x) + abs(currentNode->y - goal->y);

	return distance;
}

//bool CSearchAStar::CompareCoords(unique_ptr<SNode>& lhs, unique_ptr<SNode>& rhs)
//{
//	return lhs->score < rhs->score;
//}




