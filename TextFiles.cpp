// TextFiles.cpp
// ===============
//
// Loads Map Files and starting and finishing locations 
//

#include "Constants.h"




int LoadMap(string mapFile, int data[])
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
		for (int i = 0; i < x; i++)
		{
			for (int i = 0; i < y; i++)
			{
				infile >> data[i];
			}
			infile >> data[i];
		}
	}
	return(x, y);
}