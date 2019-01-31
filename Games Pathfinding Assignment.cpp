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
	string mapChosen;
	string coordinatesChosen;

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
	string mapSuffix = "Map.txt";
	string coordinateSuffix = "Coords.txt";
	IFont* myFont = myEngine->LoadFont("Comic Sans MS", 36);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		// need to create a for loop to create blocks, depending on the type change its skin 
		myFont->Draw(mapChosen, 1100, 50);
		myFont->Draw(coordinatesChosen, 1100, 100);

		switch (currentStateS)
		{
			case mapSelect:
			{
				myFont->Draw("stage 1", 200, 670);
				if (fileSearchFinished == false)
				{
					for (int i = 0; i < 25; i++)
					{
						mapName = (char('a' + i)) + mapSuffix;
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

				mapChosen = availableMaps[mapCounter];

				if (myEngine->KeyHit(Key_Up))
				{
					if (mapCounter == 0)
					{
						mapCounter = availableMaps.size() - 1;
					}
					else
					{
						mapCounter--;
					}

				}

				if (myEngine->KeyHit(Key_Down))
				{
					if (mapCounter == availableMaps.size() - 1)
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
					if (!costMap.empty())
					{
						costMap.clear();
					}
					LoadMap(availableMaps[mapCounter], costMap, mapXLength, mapYLength);
					fileSearchFinished = false;
					currentStateS = coordinateSelect;
				}

				break;
			}
			case coordinateSelect:
			{
				myFont->Draw("stage 2", 200, 670);
				if (!fileSearchFinished)
				{
					for (int i = 0; i < 25; i++)
					{
						coordinateFile = (char('a' + i)) + coordinateSuffix;
						ifstream ifile(coordinateFile);
						if (ifile)
						{
							availableCoordinates.push_back(coordinateFile);
						}
					}

					fileSearchFinished = true;
					amountOfCoordinateMaps = availableCoordinates.size();
				}

				coordinatesChosen = availableCoordinates[coordinateMapCounter];

				
				if (myEngine->KeyHit(Key_Up))
				{
					if (coordinateMapCounter == 0)
					{
						coordinateMapCounter = availableCoordinates.size() - 1;
					}
					else
					{
						coordinateMapCounter--;
					}

				}

				if (myEngine->KeyHit(Key_Down))
				{
					if (coordinateMapCounter == availableCoordinates.size() - 1)
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
					LoadCoordinates(availableCoordinates[coordinateMapCounter], start, goal);
					fileSearchFinished = false;
					currentStateS = algorithmSelect;
				}

				if (myEngine->KeyHit(Key_Back))
				{
					coordinatesChosen = "";
					currentStateS = mapSelect;
				}
				break;
			}
			case algorithmSelect:
			{
				myFont->Draw("stage 3", 200, 670);
				
				if (myEngine->KeyHit(Key_Return))
				{
					currentStateS = mapCreation;
				}

				if (myEngine->KeyHit(Key_Back))
				{
					currentStateS = coordinateSelect;
				}
				break;
			}
			case mapCreation:
			{
				myFont->Draw("stage 4", 200, 670);
				if (!map.empty())
				{
					clearMaps(costMap, map, mapXLength, mapYLength, blockMesh);
					CreateModels(costMap, map, blockMesh, mapXLength, mapYLength);
				}

				CreateModels(costMap, map, blockMesh, mapXLength, mapYLength);
				

				currentStateS = algorithmRunning;

				break;
			}
			case algorithmRunning:
			{
				myFont->Draw("stage 5", 200, 670);


				if (BreadthFirstSearch->FindPath(costMap, start, goal, path))
				{
					cout << "working/n";
					currentStateS = pathFound;
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

		
	


		
		

		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
