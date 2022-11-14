#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "Grid.h"
#include <vector>
#include <stack>
#include <iostream>

#define X_MAX 1000
#define X_STEP 10
#define Y_MAX 1000
#define Y_STEP 10

class Pathfinding {

public:
	Pathfinding();
	vector< Grid::Node*> FindPath(Grid::Node& _startNode, Grid::Node& _targetNode, Grid& grid);

private:
	bool isValid(int x, int y, Grid& grid);
	bool isDestination(int x, int y, Grid::Node dest);
	double calculateH(int x, int y, Grid::Node dest);
	vector<Grid::Node*> makePath(Grid& grid, Grid::Node dest);
	
};

#endif