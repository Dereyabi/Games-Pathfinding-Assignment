// Games Pathfinding Assignment.cpp: A program using the TL-Engine


#include "Definitions.h"
using namespace tle;






void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "./Media" );

	/**** Set up your scene here ****/
	//variables
	int mapXLength;
	int mapYLength;
	float cameraXPos = 50.0f, cameraYPos = 150.0f, cameraZPos = 45.0f, cameraRotation = 90.0f;

	TerrainMap costMap;

	//camera
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual, cameraXPos, cameraYPos, cameraZPos);
	myCamera->RotateLocalX(cameraRotation);
	
	//creation of models
	IMesh* blockMesh = myEngine->LoadMesh("Cube.x");


	//vectors
	vector<vector<IModel*>> map;

	string mapName = "dMap.txt";	//whatever the user selects when i get that part set up

	LoadMap("dMap.txt", costMap, mapXLength, mapYLength);
	CreateModels(costMap, map, blockMesh, mapXLength, mapYLength);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// need to create a for loop to create blocks, depending on the type change its skin 


		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
