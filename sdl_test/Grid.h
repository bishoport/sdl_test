#ifndef GRID_H
#define GRID_H

#include <SDL.h>
#include <string>
#include <functional>
#include <vector>
#include <iostream>
#include <sstream>
#include "Renderer.h"
//#include "Character.h"

using namespace std;

typedef struct Vector2
{
	int x;
	int y;
}Vector2;


typedef struct Transform
{
	Vector2 position;
	Vector2 scale;
}Transform;



class Grid {

public:
	static const int SIZE = 10;
	static const int SIZE_NODE = 50;



	struct Node
	{
		bool isActive;
		bool isWay;
		bool isPlayerNode;
		bool isEnemyNode;
		int y;
		int x;
		Vector2 centerPoint;
		int parentX;
		int parentY;
		float gCost;
		float hCost;
		float fCost;
		vector<Node*> fourNeighbors;
		vector<Node*> eightNeighbors;

		void* enemyInNode = nullptr;
	};

	Node* maze[SIZE][SIZE];
	vector<Node*> path;

	string mazeFile =
	"o x x x x x x x o o "
	"x x x x x x x x x x "
	"x x x x x x x x x x "
	"x x o o o o o o x x "
	"x o o x x x x o o x "
	"o o o o o o o o o x "
	"o o o o o o o o o o "
	"x x x x x x x x x x "
	"x x x x x x x x x x "
	"x x x x x x x x x x ";

	SDL_Color xColor = { 27,127,122,255 };
	SDL_Color wayColor = { 8,151,180,255 };
	SDL_Color activeColor = { 242, 205, 172,255 };
	SDL_Color playerColor = { 255, 0, 0,255 };
	SDL_Color emenyColor = { 0, 255, 0,255 };

	//Constructor
	Grid();

	void activeNode(int x, int y);
	void displayMaze();
	Node* getNodeByCoordinates(int x, int y);
	vector< Node*> GetMaxWalk(int x, int y);

private:
	vector< Grid::Node*> GetFourNeighbors(int x, int y);
	vector< Grid::Node*> GetEightNeighbors(int x, int y);
};


inline bool operator < (const Grid::Node& lhs, const Grid::Node& rhs)
{//We need to overload "<" to put our struct into a set
	return lhs.fCost < rhs.fCost;
}


#endif //GRID_H
