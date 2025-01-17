#include "Grid.h"
#include <iostream>

using namespace std;

Grid::Grid() {
	stringstream ss(mazeFile, ios::in);

	for (int column = 0; column < SIZE; column++)
	{
		for (int row = 0; row < SIZE; row++)
		{
			char buffer;
			Node* n = new Node();
			ss >> buffer;
			
			if (buffer == 'o')
			{
				n->isWay = true;
			}


			n->fCost = FLT_MAX;
			n->gCost = FLT_MAX;
			n->hCost = FLT_MAX;
			n->parentX = -1;
			n->parentY = -1;
			n->x = row;
			n->y = column;

			maze[column][row] = n;

			//Center node in scene
			maze[column][row]->centerPoint.x = row * SIZE_NODE - (SIZE_NODE / 2);
			maze[column][row]->centerPoint.y = column * SIZE_NODE - (SIZE_NODE / 2);
		}
	}

	//Set fourNeighbors
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			maze[i][j]->fourNeighbors = GetFourNeighbors(i, j);
			maze[i][j]->eightNeighbors = GetEightNeighbors(i, j);
		}
	}

	cout << "YA" << endl;
}


vector< Grid::Node*> Grid::GetFourNeighbors(int x, int y)
{
	vector< Grid::Node*> temp_neighbors;

	int checkX;
	int checkY;
	
	int step = 1;

	//Check Right
	checkX = x + step;
	checkY = y;

	if (checkX >= 0 && checkX < Grid::SIZE)
	{
		if (checkY >= 0 && checkY < Grid::SIZE)
		{
			temp_neighbors.push_back((maze[checkX][checkY]));
		}
	}

	////Check Left
	checkX = x - step;
	checkY = y;

	if (checkX >= 0 && checkX < Grid::SIZE)
	{
		if (checkY >= 0 && checkY < Grid::SIZE)
		{
			temp_neighbors.push_back((maze[checkX][checkY]));
		}
	}

	//Check Top
	checkX = x;
	checkY = y + step;

	if (checkX >= 0 && checkX < Grid::SIZE)
	{
		if (checkY >= 0 && checkY < Grid::SIZE)
		{
			temp_neighbors.push_back((maze[checkX][checkY]));
		}
	}

	//Check Bottom
	checkX = x;
	checkY = y - step;

	if (checkX >= 0 && checkX < Grid::SIZE)
	{
		if (checkY >= 0 && checkY < Grid::SIZE)
		{
			temp_neighbors.push_back((maze[checkX][checkY]));
		}
	}

	return temp_neighbors;
	//cout << node.fourNeighbors.size() << endl;
}
vector< Grid::Node*> Grid::GetEightNeighbors(int posX, int posY)
{
	vector< Grid::Node*> temp_neighbors;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (x == 0 && y == 0)
				continue;

			int checkX = (maze[posX][posY])->x + x;
			int checkY = (maze[posX][posY])->y + y;

			if (checkX >= 0 && checkX < SIZE && checkY >= 0 && checkY < SIZE)
			{
				temp_neighbors.push_back((maze[checkX][checkY]));
			}
		}
	}
	return temp_neighbors;
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
	if (x > SIZE - 1 || x < 0 || y > SIZE - 1 || y < 0)
	{
		cout << "x o y est�n fuera de limites" << endl;
	}
	else
	{
		maze[x][y]->isActive = true;
	}
}



void Grid::displayMaze()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			SDL_Rect rectNode = { j * SIZE_NODE,i * SIZE_NODE , SIZE_NODE , SIZE_NODE };

			if (maze[i][j]->isWay)
			{
				SDL_SetRenderDrawColor(getRenderer(), xColor.r, xColor.g, xColor.b, xColor.a);
			}
			else
			{
				SDL_SetRenderDrawColor(getRenderer(), wayColor.r, wayColor.g, wayColor.b, wayColor.a);
			}

			if (maze[i][j]->isActive)
			{
				SDL_SetRenderDrawColor(getRenderer(), activeColor.r, activeColor.g, activeColor.b, activeColor.a);
			}

			if (maze[i][j]->isPlayerNode)
			{
				SDL_SetRenderDrawColor(getRenderer(), playerColor.r, playerColor.g, playerColor.b, playerColor.a);
			}

			if (maze[i][j]->isEnemyNode)
			{
				SDL_SetRenderDrawColor(getRenderer(), emenyColor.r, emenyColor.g, emenyColor.b, emenyColor.a);
			}

			SDL_RenderFillRect(getRenderer(), &rectNode);

			//Draw center points in Node
			SDL_SetRenderDrawColor(getRenderer(), 255, 0, 0, 255);
			SDL_RenderDrawPoint(getRenderer(), maze[i][j]->centerPoint.x, maze[i][j]->centerPoint.y);
		}
	}
	
}