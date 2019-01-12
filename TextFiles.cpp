// TextFiles.cpp
// ===============
//
// Loads Map Files and starting and finishing locations 
//

#include "Constants.h"




int LoadMap()
{
	ifstream infile(mapFile);
	if (!infile)
	{
		return(LoadError);
	}
	else
	{
		infile >> maxSize;
		for (int i = 0; i < maxSize; i++)
		{
			infile >> data[i];
		}
	}
	return();
}