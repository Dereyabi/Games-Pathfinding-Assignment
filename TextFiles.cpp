// TextFiles.cpp
// ===============
//
// Loads Map Files and starting and finishing locations 
//

#include "Constants.h"




int LoadMap(string mapFile, vector<vector<int>>)
{
	int x, y;

	ifstream infile(mapFile);
	if (!infile)
	{
		return(LoadError);
	}
	else
	{
		infile >> x;
		infile >> y;
		
	}
	return(x, y);
}