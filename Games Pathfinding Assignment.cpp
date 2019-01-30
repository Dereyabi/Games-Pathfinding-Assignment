// Games Pathfinding Assignment.cpp: A program using the TL-Engine


#include "Definitions.h"
#include "Search.h"
#include "SearchFactory.h"
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
	bool fileSearchFinished = false;
	int amountOfMaps;
	int mapCounter = 0;
	int amountOfCoordinateMaps;
	int coordinateMapCounter = 0;

	TerrainMap costMap;

	//camera
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual, cameraXPos, cameraYPos, cameraZPos);
	myCamera->RotateLocalX(cameraRotation);
	
	//states
	enum searchStates { mapSelect, coordinateSelect, algorithmSelect, mapCreation, algorithmRunning, pathFound };
	searchStates currentStateS = mapSelect;

	//creation of models
	IMesh* blockMesh = myEngine->LoadMesh("Cube.x");

	//vectors
	vector<vector<IModel*>> map;
	vector<string> availableMaps;
	vector<string> availableCoordinates;

	string mapName;	//whatever the user selects when i get that part set up
	string coordinateFile;

	unique_ptr<SNode> start(new SNode);
	unique_ptr<SNode> goal(new SNode);
	NodeList path;

	
	

	ISearch* BreadthFirstSearch = NewSearch(BreadthFirst);

	string alphabet = ("a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z");
	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// need to create a for loop to create blocks, depending on the type change its skin 
		
		switch (currentStateS)
		{
			case mapSelect:
			{
				

				if (!fileSearchFinished)
				{
					for (int i = 0; i < 25; i++)
					{
						mapName = alphabet[i] + "Map.txt";
						ifstream ifile(mapName);
						if (ifile)
						{
							availableMaps.push_back(mapName);
						}
					}

					fileSearchFinished = true;
					amountOfMaps = availableMaps.size();
				}

				myFont->Draw(availableMaps[mapCounter], 500, 670);

				if (myEngine->KeyHit(Key_Up))
				{
					if (mapCounter == 0)
					{
						mapCounter = amountOfMaps;
					}
					else
					{
						mapCounter--;
					}

				}

				if (myEngine->KeyHit(Key_Down))
				{
					if (mapCounter == amountOfMaps)
					{
						mapCounter = 0;
					}
					else
					{
						mapCounter++;
					}

				}

				if (myEngine->KeyHit(Key_Return))
				{
					LoadMap(mapName, costMap, mapXLength, mapYLength);
					fileSearchFinished = false;
					currentStateS = coordinateSelect;
				}
			}
			case coordinateSelect:
			{
				if (!fileSearchFinished)
				{
					for (int i = 0; i < 25; i++)
					{
						coordinateFile = alphabet[i] + "Coord.txt";
						ifstream ifile(coordinateFile);
						if (ifile)
						{
							availableCoordinates.push_back(coordinateFile);
						}
					}

					fileSearchFinished = true;
					amountOfCoordinateMaps = availableCoordinates.size();
				}

				myFont->Draw(availableCoordinates[coordinateMapCounter], 500, 670);

				if (myEngine->KeyHit(Key_Up))
				{
					if (mapCounter == 0)
					{
						coordinateMapCounter = amountOfCoordinateMaps;
					}
					else
					{
						coordinateMapCounter--;
					}

				}

				if (myEngine->KeyHit(Key_Down))
				{
					if (coordinateMapCounter == amountOfCoordinateMaps)
					{
						coordinateMapCounter = 0;
					}
					else
					{
						coordinateMapCounter++;
					}

				}

				if (myEngine->KeyHit(Key_Return))
				{
					LoadCoordinates(coordinateFile, start, goal);
					fileSearchFinished = false;
					currentStateS = algorithmSelect;
				}
			}
			case algorithmSelect:
			{
				currentStateS = mapCreation;
			}
			case mapCreation:
			{
				CreateModels(costMap, map, blockMesh, mapXLength, mapYLength);
				system("pause");
			}
			case algorithmRunning:
			{
				BreadthFirstSearch->FindPath(costMap, start, goal, path);
			}
			case pathFound:
			{

			}
		}

		//coordinateSelect, algorithmSelect, algorithmRunning, pathFound
	



		

		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
