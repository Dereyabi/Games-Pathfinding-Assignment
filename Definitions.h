//Jay Stewart
// Definitions.h
// =============
//
// Type definitions to support path finding 
//

#pragma once
#include <TL-Engine.h>
#include <vector>
#include <deque>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace tle;

// Represents a node in the search tree.
struct SNode
{
  int x;				// x coordinate
  int y;				// y coordinate
  int terrainCost = 0;
  int score = 0;			// used in more complex algorithms
  SNode* parent = 0;	// note use of raw pointer here
};

struct SCurrentNode
{
	int x;			// x coordinate
	int y;			// y coordinate
};

// Lists of nodes (e.g Open, Closed and Paths) are implemented as double-ended queues
using NodeList = deque<unique_ptr<SNode>>;

// Terrain costs for map squares
enum ETerrainCost
{
  Clear = 1,
  Wood = 2,
  Water  = 3,
  Wall  = 0
};

enum EBlockSkins
{
	

};

// Maps of any size are implemented as 2D vectors
using TerrainMap = vector<vector<ETerrainCost>>;

void LoadMap(string mapFile, TerrainMap& costMap, int& xSize, int& ySize);
void LoadCoordinates(string coordinateFile, unique_ptr<SNode>& start, unique_ptr<SNode>& goal);
void CreateModels(TerrainMap& costMap, vector<vector<IModel*>>& modelMap, IMesh* blockMesh, int mapXSize, int mapYSize);
void clearMaps(TerrainMap& costMap, vector<vector<IModel*>>& modelMap, int mapXSize, int mapYSize, IMesh* blockMesh, unique_ptr<SNode>& start, unique_ptr<SNode>& goal);

const int LoadError = -1;
