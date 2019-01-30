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
	string suffix = "map.txt";
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
				if (fileSearchFinished == false)
				{
					for (int i = 0; i < 25; i++)
					{
						mapName = (char('a' + i)) + suffix;
						ifstream ifile(mapName);
						if (ifile)
						{
							availableMaps.push_back(mapName);
						}
						ifile.close();
					}

					fileSearchFinished = true;
					amountOfMaps = availableMaps.size();
				}

				myFont->Draw(availableMaps[mapCounter], 500, 670);

				if (myEngine->KeyHit(Key_Up))
				{


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
					if (costMap.empty)
					{
						LoadMap(availableMaps[mapCounter], costMap, mapXLength, mapYLength);
						fileSearchFinished = false;
						currentStateS = coordinateSelect;
					}
					else
					{
						//delete all existing models in the model array
						LoadMap(availableMaps[mapCounter], costMap, mapXLength, mapYLength);
						fileSearchFinished = false;
						currentStateS = coordinateSelect;
					}

				}
				break;
			}
			case coordinateSelect:
			{
				if (!fileSearchFinished)
				{
					for (int i = 0; i < 25; i++)
					{
						coordinateFile = (char('a' + i)) + suffix;
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

				if (myEngine->KeyHit(Key_Back))
				{
					currentStateS = mapSelect;
				}
				break;
			}
			case algorithmSelect:
			{
				currentStateS = mapCreation;
				if (myEngine->KeyHit(Key_Back))
				{
					currentStateS = coordinateSelect;
				}
				break;
			}
			case mapCreation:
			{
				CreateModels(costMap, map, blockMesh, mapXLength, mapYLength);
				break;
			}
			case algorithmRunning:
			{
				BreadthFirstSearch->FindPath(costMap, start, goal, path);

				if (myEngine->KeyHit(Key_Back))
				{
					currentStateS = algorithmSelect;
				}
				break;
			}
			case pathFound:
			{
				if (myEngine->KeyHit(Key_Back))
				{
					currentStateS = algorithmSelect;
				}
				break;
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
