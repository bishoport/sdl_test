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
	static const int SIZE_Y = 15;
	static const int SIZE_X = 21;
	static const int SIZE_NODE = 50;

	struct Node
	{
		bool isActive = false;
		bool isWay = false;
		bool isPlayerNode = false;
		bool isEnemyNode = false;
		int y = 0;
		int x = 0;
		Vector2 centerPoint;
		int parentX  = -1;
		int parentY = -1;
		float gCost = FLT_MAX;
		float hCost = FLT_MAX;
		float fCost = FLT_MAX;
		vector<Node*> fourNeighbors;
		vector<Node*> eightNeighbors;

		void* enemyInNode = nullptr;
	};

	Node* maze[SIZE_Y][SIZE_X]{};
	vector<Node*> path;

	
	string mazeFile =
		"xxxxxxxxxxxxxxxxxxxxx"
		"xooooooxxoooxxooxxoox"
		"xopooooxxoooxxooxxoox"
		"xoooooooooooxxooxxoox"
		"xooooooxxoooxxooxxoox"
		"xooooooxxoooooooxxoox"
		"xooooooxxooooxoooooox"
		"xxxxxxxxxxxxoxxxxxxxx"
		"xxxxxxxxxxxxoxxxxxxxx"
		"xoooooooxxoooooeoooox"
		"xoooxxooxxoooooxxooox"
		"xoooxxooxxoooooxxooox"
		"xoooxxooxxoooooxxooox"
		"xoeoxxoooooooooxxooox"
		"xxxxxxxxxxxxxxxxxxxxx";
	
	vector<Node*> startEnemiesNodes;
	Node* startPlayerNode;

	SDL_Color xColor = { 27,127,122,255 };
	SDL_Color wayColor = { 8,151,180,255 };
	SDL_Color activeColor = { 242, 205, 172,255 };
	SDL_Color playerColor = { 255, 0, 0,255 };
	SDL_Color emenyColor = { 0, 255, 0,255 };

	//Constructor
	Grid();

	void activeNode(int x, int y);
	void unActiveNode(int x, int y);
	void unActiveAllNodes();
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
