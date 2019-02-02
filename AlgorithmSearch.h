//Jay Stewart
#pragma once

#include "Definitions.h"  // Type definitions
#include "Search.h"       // Base (=interface) class definition

class CSearchBreadthFirst : public ISearch
{
	// I have not implemented any constructors or destructors.
	// Whether you need some is up to how you choose to do your implementation.

	// Constructs the path from start to goal for the given terrain
	bool FindPath(TerrainMap& terrain, unique_ptr<SNode>& start, unique_ptr<SNode>& goal, NodeList& path, int mapXLength, int mapYLength, I3DEngine* myEngine, vector<vector<IModel*>>& modelMap);
	bool ListSearch(NodeList& currentList, SNode* currentNode, SCurrentNode& currentPosition);
	ETerrainCost BoundrySearch(TerrainMap& terrain, SNode* currentNode, SCurrentNode& currentPosition, int mapXLength, int mapYLength);
};

class CSearchAStar : public ISearch
{
	bool FindPath(TerrainMap& terrain, unique_ptr<SNode>& start, unique_ptr<SNode>& goal, NodeList& path, int mapXLength, int mapYLength, I3DEngine* myEngine, vector<vector<IModel*>>& modelMap);

	bool ListSearch(NodeList& currentList, SNode* currentNode, SCurrentNode& currentPosition);

	ETerrainCost BoundrySearch(TerrainMap& terrain, SNode* currentNode, SCurrentNode& currentPosition, int mapXLength, int mapYLength);

	ETerrainCost ScoreCheck(TerrainMap& terrain, SNode* currentNode, SCurrentNode& currentPosition);

	int CSearchAStar::ManhattanDistance(SNode* currentNode, unique_ptr<SNode>& goal);

	bool CompareCoords();
};
