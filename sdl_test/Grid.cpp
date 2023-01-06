#include "Grid.h"
#include <iostream>
#include "Character.h"
#include "ReferencesManager.h"

using namespace std;

Grid::Grid() {

	stringstream ss(mazeFile, ios::in);

	for (int column = 0; column < SIZE_COLS; column++)
	{
		for (int row = 0; row < SIZE_ROWS; row++)
		{
			char buffer;
			Node* n = new Node();
			ss >> buffer;
			
			if (buffer == 'o')
			{
				n->isWay = true;
			}
			else if (buffer == 'e')
			{
				n->isWay = true;
				startEnemiesNodes.push_back(n);
			}
			else if (buffer == 'p')
			{
				n->isWay = true;
				startPlayerNode = n;
			}
			else if (buffer == 'x')
			{
				n->isWay = false;
			}
			else if (buffer == 'd')
			{
				n->isWay = false;
				n->isDoor = true;
			}

			n->fCost = FLT_MAX;
			n->gCost = FLT_MAX;
			n->hCost = FLT_MAX;
			n->parentX = -1;
			n->parentY = -1;
			n->x = row;
			n->y = column;

			n->isActive = false;
			n->isPlayerNode = false;
			n->isEnemyNode = false;
			n->enemyInNode = nullptr;


			maze[column][row] = n;

			//Center node in scene
			maze[column][row]->centerPoint.x = (row + 1) * SIZE_NODE - (SIZE_NODE / 2);
			maze[column][row]->centerPoint.y = (column + 1) * SIZE_NODE-(SIZE_NODE / 2);
		}
	}

	//Set Neighbors
	for (int column = 0; column < SIZE_COLS; column++)
	{
		for (int row = 0; row < SIZE_ROWS; row++)
		{
			maze[column][row]->fourNeighbors = GetFourNeighbors(column, row);
			maze[column][row]->eightNeighbors = GetEightNeighbors(column, row,1);
		}
	}

	Grid::loadMedia("assets/sprites/sprite_sheet_dungeon.png");

	cout << "GRID GENERADO" << endl;
}


bool Grid::loadMedia(const char* _filename)
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if (!gSpriteSheetTexture.loadFromFile(_filename))
	{
		printf("Failed to load sprite sheet texture in grid!\n");
		success = false;
	}
	else
	{
		//Set WAY sprite
		gSpriteClips[0].x = Grid::SIZE_NODE * 0;
		gSpriteClips[0].y = Grid::SIZE_NODE * 0;
		gSpriteClips[0].w = Grid::SIZE_NODE;
		gSpriteClips[0].h = Grid::SIZE_NODE;

		//Set WALL sprite
		gSpriteClips[1].x = Grid::SIZE_NODE * 1;
		gSpriteClips[1].y = Grid::SIZE_NODE * 0;
		gSpriteClips[1].w = Grid::SIZE_NODE;
		gSpriteClips[1].h = Grid::SIZE_NODE;

		//Set DOOR sprite
		gSpriteClips[2].x = Grid::SIZE_NODE * 2;
		gSpriteClips[2].y = Grid::SIZE_NODE * 0;
		gSpriteClips[2].w = Grid::SIZE_NODE;
		gSpriteClips[2].h = Grid::SIZE_NODE;
	}

	return success;
}

vector< Grid::Node*> Grid::GetFourNeighbors(int column, int row)
{
	vector< Grid::Node*> temp_neighbors;

	int checkColumn;
	int checkRow;
	
	int step = 1;

	//Check Right
	checkColumn = column + step;
	checkRow = row;

	if (checkColumn >= 0 && checkColumn < Grid::SIZE_COLS)
	{
		if (checkRow >= 0 && checkRow < Grid::SIZE_ROWS)
		{
			temp_neighbors.push_back((maze[checkColumn][checkRow]));
		}
	}

	////Check Left
	checkColumn = column - step;
	checkRow = row;

	if (checkColumn >= 0 && checkColumn < Grid::SIZE_COLS)
	{
		if (checkRow >= 0 && checkRow < Grid::SIZE_ROWS)
		{
			temp_neighbors.push_back((maze[checkColumn][checkRow]));
		}
	}

	//Check Top
	checkColumn = column;
	checkRow = row + step;

	if (checkColumn >= 0 && checkColumn < Grid::SIZE_COLS)
	{
		if (checkRow >= 0 && checkRow < Grid::SIZE_ROWS)
		{
			temp_neighbors.push_back((maze[checkColumn][checkRow]));
		}
	}

	//Check Bottom
	checkColumn = column;
	checkRow = row - step;

	if (checkColumn >= 0 && checkColumn < Grid::SIZE_COLS)
	{
		if (checkRow >= 0 && checkRow < Grid::SIZE_ROWS)
		{
			temp_neighbors.push_back((maze[checkColumn][checkRow]));
		}
	}

	return temp_neighbors;
	//cout << node.fourNeighbors.size() << endl;
}

vector< Grid::Node*> Grid::GetEightNeighbors(int column, int row, int range)
{
	vector< Grid::Node*> temp_neighbors;

	for (int x = -range; x <= range; x++)
	{
		for (int y = -range; y <= range; y++)
		{
			if (x == 0 && y == 0)
				continue;

			if (maze[column][row])
			{
				int checkRow = (maze[column][row])->x + x;
				int checkCol = (maze[column][row])->y + y;

				if (checkRow >= 0 && checkRow < SIZE_ROWS && checkCol >= 0 && checkCol < SIZE_COLS)
				{
					temp_neighbors.push_back((maze[checkCol][checkRow]));
				}
			}
		}
	}
	return temp_neighbors;
}

void Grid::setDiscoveredByRange(int column, int row, int range)
{
	for (int r = -range; r < range + 1; r++)
	{
		if (maze[row+ r][column])
		{
			for (int i = 0; i < maze[row+ r][column]->eightNeighbors.size(); i++)
			{
				maze[row + r][column]->eightNeighbors[i]->isDiscovered = true;
			}
		}
		/*if (maze[row + r][column + r])
		{
			for (int i = 0; i < maze[row + r][column + r]->eightNeighbors.size(); i++)
			{
				maze[row + r][column + r]->eightNeighbors[i]->isDiscovered = true;
			}
		}*/
		if (maze[row][column + r])
		{
			for (int i = 0; i < maze[row][column + r]->eightNeighbors.size(); i++)
			{
				maze[row][column + r]->eightNeighbors[i]->isDiscovered = true;
			}
		}


		if (maze[row - r][column])
		{
			for (int i = 0; i < maze[row - r][column]->eightNeighbors.size(); i++)
			{
				maze[row - r][column]->eightNeighbors[i]->isDiscovered = true;
			}
		}
		/*if (maze[row - r][column - r])
		{
			for (int i = 0; i < maze[row - r][column - r]->eightNeighbors.size(); i++)
			{
				maze[row - r][column - r]->eightNeighbors[i]->isDiscovered = true;
			}
		}*/
		if (maze[row][column - r])
		{
			for (int i = 0; i < maze[row][column - r]->eightNeighbors.size(); i++)
			{
				maze[row][column - r]->eightNeighbors[i]->isDiscovered = true;
			}
		}
	}
}

vector< Grid::Node*> Grid::GetMaxWalk(int x, int y)
{
	vector< Grid::Node*> temp_neighbors;


	/*for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			array[i - 1][j - 1]
				array[i - 1][j]
				array[i - 1][j + 1]

				array[i][j - 1]
				array[i][j + 1]

				array[i + 1][j - 1]
				array[i + 1][j]
				array[i + 1][j + 1]
		}
	}*/

	return temp_neighbors;
	//cout << node.fourNeighbors.size() << endl;
}
Grid::Node* Grid::getNodeByCoordinates(int x, int y)
{
	return (maze[x][y]);
}






void Grid::activeNode(int x, int y)
{
	if (x > SIZE_COLS - 1 || x < 0 || y > SIZE_COLS - 1 || y < 0)
	{
		cout << "x o y están fuera de limites" << endl;
	}
	else
	{
		maze[x][y]->isActive = true;
	}
}

void Grid::unActiveNode(int x, int y)
{
	if (x > SIZE_COLS - 1 || x < 0 || y > SIZE_COLS - 1 || y < 0)
	{
		cout << "x o y están fuera de limites" << endl;
	}
	else
	{
		maze[x][y]->isActive = false;
	}
}

void Grid::unActiveAllNodes()
{
	for (int i = 0; i < SIZE_COLS; i++)
	{
		for (int j = 0; j < SIZE_ROWS; j++)
		{
			maze[i][j]->isActive = false;
		}
	}
}



void Grid::displayMaze()
{
	for (int i = 0; i < SIZE_COLS; i++)
	{
		for (int j = 0; j < SIZE_ROWS; j++)
		{

			if (maze[i][j]->isDiscovered == true)
			{
				SDL_Rect rectNode = { j * SIZE_NODE,i * SIZE_NODE , SIZE_NODE , SIZE_NODE };

				if (maze[i][j]->isWay)
				{
					//SDL_SetRenderDrawColor(getRenderer(), xColor.r, xColor.g, xColor.b, xColor.a);
					gSpriteSheetTexture.render(rectNode.x, rectNode.y, SDL_FLIP_NONE, &gSpriteClips[0]);
				}
				else
				{
					if (maze[i][j]->isDoor)
					{
						gSpriteSheetTexture.render(rectNode.x, rectNode.y, SDL_FLIP_NONE, &gSpriteClips[2]);
					}
					else
					{
						gSpriteSheetTexture.render(rectNode.x, rectNode.y, SDL_FLIP_NONE, &gSpriteClips[1]);
					}

				}



				//if (maze[i][j]->isPlayerNode)
				//{
				//	SDL_SetRenderDrawColor(getRenderer(), playerColor.r, playerColor.g, playerColor.b, playerColor.a);
				//	SDL_RenderFillRect(getRenderer(), &rectNode);
				//}

				//if (maze[i][j]->isActive)
				//{
				//	SDL_SetRenderDrawColor(getRenderer(), activeColor.r, activeColor.g, activeColor.b, activeColor.a);
				//	SDL_RenderFillRect(getRenderer(), &rectNode);
				//}

				//if (maze[i][j]->isEnemyNode)
				//{
				//	SDL_SetRenderDrawColor(getRenderer(), emenyColor.r, emenyColor.g, emenyColor.b, emenyColor.a);
				//}

				

				//Draw center points in Node
				//SDL_SetRenderDrawColor(getRenderer(), 255, 0, 0, 255);
				//SDL_RenderDrawPoint(getRenderer(), maze[i][j]->centerPoint.x, maze[i][j]->centerPoint.y);
			}
		}
	}
}