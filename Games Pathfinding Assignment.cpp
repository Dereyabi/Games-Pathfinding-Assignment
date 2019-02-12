//Jay Stewart
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
	float cameraXPos = 50.0f, cameraYPos = 50.0f, cameraZPos = -200.0f;
	bool fileSearchFinished = false;
	int amountOfMaps;
	int mapCounter = 0;
	int amountOfCoordinateMaps;
	int coordinateMapCounter = 0;
	string mapChosen;
	string coordinatesChosen;
	bool pathFoundCheck = false;
	bool algorithmPicker = false;

	TerrainMap costMap;

	//camera
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual, cameraXPos, cameraYPos, cameraZPos);
	
	//states
	enum searchStates { mapAndCoordSelect, algorithmSelect, mapCreation, algorithmRunning, pathFound };
	searchStates currentStateS = mapAndCoordSelect;

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

	
	

	ISearch* Search;
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
		if (pathFoundCheck)
		{
			myFont->Draw("Path Found", 1100, 150);
		}
		else
		{
			myFont->Draw("No Path", 1100, 150);
		}
		if (!algorithmPicker)
		{
			myFont->Draw("BreadthFirst", 1100, 200);
		}
		if (algorithmPicker)
		{
			myFont->Draw("AStar", 1100, 200);
		}
		

		switch (currentStateS)
		{
			case mapAndCoordSelect:
			{
				myFont->Draw("Select a Map and Coordinate File", 200, 670);
				if (!fileSearchFinished)
				{
					for (int i = 0; i < 25; i++)
					{
						//search available Maps
						mapName = (char('a' + i)) + mapSuffix;
						ifstream ifile(mapName);
						if (ifile)
						{
							availableMaps.push_back(mapName);
						}
						ifile.clear();

						//search available Coordinates
						coordinateFile = (char('a' + i)) + coordinateSuffix;
						ifstream jfile(coordinateFile);
						if (jfile)
						{
							availableCoordinates.push_back(coordinateFile);
						}
						jfile.close();
					}

					fileSearchFinished = true;
					amountOfMaps = availableMaps.size();
					amountOfCoordinateMaps = availableCoordinates.size();
				}

				mapChosen = availableMaps[mapCounter];

				coordinatesChosen = availableCoordinates[coordinateMapCounter];

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
					if (mapCounter == availableMaps.size() - 1)
					{
						mapCounter = 0;
					}
					else
					{
						mapCounter++;
					}

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
					start.reset(new SNode);
					goal.reset(new SNode);

					if (!costMap.empty())
					{
						costMap.clear();
					}

					LoadMap(availableMaps[mapCounter], costMap, mapXLength, mapYLength);
					LoadCoordinates(availableCoordinates[coordinateMapCounter], start, goal);
					fileSearchFinished = false;
					currentStateS = mapCreation;
				}

				break;
			}
			case mapCreation:
			{
				myFont->Draw("map being created", 200, 670);
				if (!map.empty())
				{
					clearMaps(costMap, map, mapXLength, mapYLength, blockMesh, start, goal);
					CreateModels(costMap, map, blockMesh, mapXLength, mapYLength);
				}

				CreateModels(costMap, map, blockMesh, mapXLength, mapYLength);
				map[start->y][start->x]->SetSkin("checked1.jpg");
				map[goal->y][goal->x]->SetSkin("checked1.jpg");

				currentStateS = algorithmSelect;

				break;
			}
			case algorithmSelect:
			{
				myFont->Draw("Select an Algorithm", 200, 670);

				if (myEngine->KeyHit(Key_Up))
				{
					algorithmPicker = !algorithmPicker;
				}

				if (myEngine->KeyHit(Key_Down))
				{
					algorithmPicker = !algorithmPicker;
				}

				if (myEngine->KeyHit(Key_Return))
				{
					if (algorithmPicker == false)
					{
						cout << "BreadthFirst" << endl;
						Search = NewSearch(BreadthFirst);
					}
					else if (algorithmPicker == true)
					{
						cout << "AStar" << endl;
						Search = NewSearch(AStar);
					}

					currentStateS = algorithmRunning;
				}

				if (myEngine->KeyHit(Key_Back))
				{
					currentStateS = mapAndCoordSelect;
				}
				break;
			}
			case algorithmRunning:
			{
				myFont->Draw("stage 5", 200, 670);


				//if (BreadthFirstSearch->FindPath(costMap, start, goal, path, mapXLength, mapYLength, myEngine, map))
				if (Search->FindPath(costMap, start, goal, path, mapXLength, mapYLength, myEngine, map))
				{
					pathFoundCheck = true;
					currentStateS = pathFound;
				}
				else
				{
					pathFoundCheck = false;
					currentStateS = pathFound;
				}

				
				break;
			}
			case pathFound:
			{
				ofstream outfile("Bestpath.txt");
				for (auto it = path.begin(); it != path.end(); ++it)
				{
					map[(*it)->y][(*it)->x]->SetSkin("path.jpg");
					outfile << (*it)->x << ", " << (*it)->y << endl;
				}

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
