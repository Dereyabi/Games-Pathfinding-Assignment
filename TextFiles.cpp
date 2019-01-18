// TextFiles.cpp
// ===============
//
// Loads Map Files and starting and finishing locations 
//

#include "Definitions.h"




void LoadMap(string mapFile, TerrainMap &costMap)
{

	ifstream infile(mapFile);
	if (!infile)
	{
		cout << "map not found" << endl;
	}

	int xSize = 0;
	int ySize = 0;



	infile >> xSize >> ySize;

	char blockType;



	for (int i = 0; i < xSize; i++)
	{
		vector<ETerrainCost> row;
		ETerrainCost type;

		for (int j = 0; j < ySize; j++)
		{
			infile >> blockType;

			if (blockType == 0)
			{
				type = Wall;
			}
			else if (blockType == 1)
			{
				type = Clear;
			}
			else if (blockType == 2)
			{
				type = Wood;
			}
			else if (blockType == 3)
			{
				type = Water;
			}

			cout << blockType;			//DEBUGGING

			row.push_back(type);
		}
		costMap.push_back(row);
		cout << endl;					//DEBUGGING
	}
}