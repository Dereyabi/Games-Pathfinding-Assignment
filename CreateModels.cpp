// CreateModels.cpp
// ===============
//
// Creates a visual representation of the map and sets the skins of blocks
//

#include "Definitions.h"

void CreateModels(TerrainMap& costMap, vector<vector<IModel*>>& modelMap, IMesh* blockMesh, int mapXSize, int mapYSize)
{
	vector<IModel*> row;
	float modelSizeMultiplier = 10.0f;

	for (int i = 0; i < mapYSize; i++)
	{
		for (int j = 0; j < mapXSize; j++)
		{
			row.push_back(move(blockMesh->CreateModel(j*modelSizeMultiplier, 0.0f, i*modelSizeMultiplier)));
		}
		modelMap.push_back(move(row));
	}

	for (int k = 0; k < mapYSize; k++)
	{
		for (int l = 0; l < mapXSize; l++)
		{
			if (costMap[k][l] == Wall)
			{
				modelMap[k][l]->SetSkin("brick1.jpg");
			}
			else if (costMap[k][l] == Clear)
			{
				modelMap[k][l]->SetSkin("wood2.jpg");
			}
			else if (costMap[k][l] == Wood)
			{
				modelMap[k][l]->SetSkin("wood1.jpg");
			}
			else if (costMap[k][l] == Water)
			{
				modelMap[k][l]->SetSkin("tiles1.jpg");
			}
		}
	}
}

void clearMaps(TerrainMap costMap, vector<vector<IModel*>> modelMap, int mapXSize, int mapYSize, IMesh* blockMesh)
{
	for (int k = 0; k < mapYSize; k++)
	{
		for (int l = 0; l < mapXSize; l++)
		{
			blockMesh->RemoveModel(modelMap[k][l]);
		}
	}
	modelMap.clear();
	costMap.clear();
}