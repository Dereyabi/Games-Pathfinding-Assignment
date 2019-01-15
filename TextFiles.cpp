// TextFiles.cpp
// ===============
//
// Loads Map Files and starting and finishing locations 
//

#include "Constants.h"




void LoadMap(string mapFile, vector<vector<char>> map)
{

	ifstream infile(mapFile);
	if (!infile)
	{
		cout << "map not found" << endl;
	}

	int xSize = 0;
	int ySize = 0;



	infile >> xSize;
	infile >> ySize;

	char blockType;



	for (int i = 0; i < xSize; i++)
	{
		vector<char> row;

		for (int j = 0; j < ySize; j++)
		{
			infile >> blockType;

			cout << blockType;

			row.push_back(blockType);
		}
		cout << endl;
	}
}