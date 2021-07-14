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
	bool firstCoord = true;
	int coordCounterX = 0;
	int coordCounterY = 0;
	bool coordNotSelected = true;
	bool coordMethod = false;

	TerrainMap costMap;

	//camera
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual, cameraXPos, cameraYPos, cameraZPos);
	
	//states
	enum searchStates { mapSelect, customOrBaseCoord, coordSelect, customCoordSelect, algorithmSelect, mapCreation, algorithmRunning, pathFound };
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

	IModel* coordSelectModel = blockMesh->CreateModel(-1000, -1000, -10);
	coordSelectModel->SetSkin("CustomCord.jpg");
	

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
		myFont->Draw(mapChosen, 1000, 50);

		if (pathFoundCheck && currentStateS == pathFound)
		{
			myFont->Draw("Path Found", 1000, 150);
		}
		else if (!pathFoundCheck && currentStateS == pathFound)
		{
			myFont->Draw("No Path", 1000, 150);
		}
		if (algorithmPicker == false && currentStateS >= 3)
		{
			myFont->Draw("BreadthFirst", 1000, 200);
		}
		else if (algorithmPicker == true && currentStateS >= 3)
		{
			myFont->Draw("AStar", 1000, 200);
		}
		if (coordMethod == false && currentStateS == customOrBaseCoord)
		{
			myFont->Draw("Base Coordinates", 1000, 100);
		}
		else if (coordMethod == true && currentStateS == customOrBaseCoord)
		{
			myFont->Draw("Custom Coordinates", 1000, 100);
		}
		
		if (currentStateS >= 3 && start != NULL)
		{
			myFont->Draw("Start:", 1000, 100);
			myFont->Draw("(", 1100, 100);
			myFont->Draw(to_string(start->x), 1110, 100);				//is a nullpointer becasue start and goal get sent by referernce and put on the open list, therefore its null
			myFont->Draw(",", 1125, 100);
			myFont->Draw(to_string(start->y), 1135, 100);				//same for this and the end too
			myFont->Draw(")", 1150, 100);
		}

		if (currentStateS >= 3 && currentStateS < 7 && goal != NULL)
		{
			myFont->Draw("End:", 1000, 150);
			myFont->Draw("(", 1100, 150);
			myFont->Draw(to_string(goal->x), 1110, 150);
			myFont->Draw(",", 1125, 150);
			myFont->Draw(to_string(goal->y), 1135, 150);
			myFont->Draw(")", 1150, 150);
		}

		switch (currentStateS)
		{
			case mapSelect:
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
				
				if (!map.empty())
				{
					clearMaps(costMap, map, mapXLength, mapYLength, blockMesh, start, goal);
					CreateModels(costMap, map, blockMesh, mapXLength, mapYLength);
					
				}

				CreateModels(costMap, map, blockMesh, mapXLength, mapYLength);

				currentStateS = customOrBaseCoord;

				break;
			}
			case customOrBaseCoord:
			{

				if (myEngine->KeyHit(Key_Up))
				{
					coordMethod = !coordMethod;
				}

				if (myEngine->KeyHit(Key_Down))
				{
					coordMethod = !coordMethod;
				}

				if (myEngine->KeyHit(Key_Return))
				{
					if (coordMethod == false)
					{
						currentStateS = coordSelect;
					}
					else if (coordMethod == true)
					{
						start->x = 0;
						start->y = 0;
						goal->x = 0;
						goal->y = 0;
						coordCounterX = 0;
						coordCounterY = 0;
						coordSelectModel->SetPosition(0, 0, -10);
						currentStateS = customCoordSelect;
					}
				}

				break;
			}
			case coordSelect:
			{
				
				coordinatesChosen = availableCoordinates[coordinateMapCounter];
				map[start->y][start->x]->SetSkin("checked1.jpg");
				map[goal->y][goal->x]->SetSkin("checked1.jpg");

				currentStateS = algorithmSelect;
			}
			case customCoordSelect:
			{
				if (coordCounterX == mapXLength)
				{
					coordCounterX = 0;
				}
				if (coordCounterY == mapYLength)
				{
					coordCounterY = 0;
				}
				if (coordCounterX < 0)
				{
					coordCounterX = mapXLength - 1;
				}
				if (coordCounterY < 0)
				{
					coordCounterY = mapYLength - 1;
				}

				if (myEngine->KeyHit(Key_Up))
				{
					coordCounterY++;
					coordSelectModel->SetPosition(10 * coordCounterX, 10 * coordCounterY, -10);
				}
				if (myEngine->KeyHit(Key_Down))
				{
					coordCounterY--;
					coordSelectModel->SetPosition(10 * coordCounterX, 10 * coordCounterY, -10);
				}
				if (myEngine->KeyHit(Key_Left))
				{
					coordCounterX--;
					coordSelectModel->SetPosition(10 * coordCounterX, 10 * coordCounterY, -10);
				}
				if (myEngine->KeyHit(Key_Right))
				{
					coordCounterX++;
					coordSelectModel->SetPosition(10 * coordCounterX, 10 * coordCounterY, -10);
				}

				if (myEngine->KeyHit(Key_Return))
				{
					
					if (firstCoord)
					{
						start->x = coordCounterX;
						start->y = coordCounterY;
						map[start->y][start->x]->SetSkin("checked1.jpg");
						coordCounterX = 0;
						coordCounterY = 0;
						coordSelectModel->SetPosition(0, 0, -10);
						firstCoord = false;
					}
					else if(!firstCoord)
					{
						goal->x = coordCounterX;
						goal->y = coordCounterY;
						map[goal->y][goal->x]->SetSkin("checked1.jpg");
						coordSelectModel->SetPosition(-1000, -1000, -10);
						currentStateS = algorithmSelect;
						
					}	
				}
				break;
			}
			case algorithmSelect:
			{
				firstCoord = true;

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
				break;
			}
			case algorithmRunning:
			{

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


				break;
			}
		}

		if (myEngine->KeyHit(Key_Back) && currentStateS != mapSelect)
		{
			currentStateS = mapSelect;
			clearMaps(costMap, map, mapXLength, mapYLength, blockMesh, start, goal);
		}

		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
