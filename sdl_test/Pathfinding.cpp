
#include "Pathfinding.h"

using namespace std;

Pathfinding::Pathfinding() {

	
}

vector< Grid::Node*> Pathfinding::FindPath(Grid::Node& _startNode, Grid::Node& _targetNode, Grid& grid)
{
    vector<Grid::Node*> empty;

    bool destinationFound = false;

    //Reset GRID
    for (int i = 0; i < Grid::SIZE; i++)
    {
        for (int j = 0; j < Grid::SIZE; j++)
        {
            grid.maze[i][j]->fCost = FLT_MAX;
            grid.maze[i][j]->gCost = FLT_MAX;
            grid.maze[i][j]->hCost = FLT_MAX;
            grid.maze[i][j]->parentX = -1;
            grid.maze[i][j]->parentY = -1;
            grid.maze[i][j]->x = i;
            grid.maze[i][j]->y = j;
            grid.maze[i][j]->isActive = false;
            grid.maze[i][j]->isPlayerNode = false;
            grid.maze[i][j]->isEnemyNode = false;
            grid.maze[i][j]->enemyInNode = nullptr;
        }
    }

    //SET START NODE
    int x = _startNode.x;
    int y = _startNode.y;
    grid.maze[x][y]->fCost = 0.0;
    grid.maze[x][y]->gCost = 0.0;
    grid.maze[x][y]->hCost = 0.0;
    grid.maze[x][y]->parentX = x;
    grid.maze[x][y]->parentY = y;

    //CLOSED SET
    bool closedSet[(X_MAX / X_STEP)][(Y_MAX / Y_STEP)];
    for (int i = 0; i < X_MAX / X_STEP; i++)
    {
        for (int j = 0; j < Y_MAX / Y_STEP; j++)
        {
            closedSet[i][j] = false;
        }
    }

    //OPEN SET
    vector<Grid::Node*> openSet;
    openSet.emplace_back(grid.maze[x][y]);


    //EMPIEZA EL ALGORITMO
	while (openSet.size() > 0)
	{
		Grid::Node* currentNode;

		do {
            float temp = FLT_MAX;
            vector<Grid::Node*>::iterator itNode;
            for (vector<Grid::Node*>::iterator it = openSet.begin();
                it != openSet.end(); it = next(it)) {
                Grid::Node* n = *it;
                if (n->fCost < temp) {
                    temp = n->fCost;
                    itNode = it;
                }
            }
            currentNode = *itNode;
            openSet.erase(itNode);
		} 
        
        while (isValid(currentNode->x, currentNode->y ,grid) == false);

        x = currentNode->x;
        y = currentNode->y;
        closedSet[x][y] = true;


        //CHECK FOUR NEIGHBORS
        for (int i = 0; i < grid.maze[x][y]->fourNeighbors.size(); i++)
        {
            double gNew, hNew, fNew;
            if (isValid(grid.maze[x][y]->fourNeighbors[i]->x, grid.maze[x][y]->fourNeighbors[i]->y, grid))
            {
                if (isDestination(grid.maze[x][y]->fourNeighbors[i]->x, grid.maze[x][y]->fourNeighbors[i]->y, _targetNode))
                {
                    //Destination found - make path
                    grid.maze[grid.maze[x][y]->fourNeighbors[i]->x][grid.maze[x][y]->fourNeighbors[i]->y]->parentX = x;
                    grid.maze[grid.maze[x][y]->fourNeighbors[i]->x][grid.maze[x][y]->fourNeighbors[i]->y]->parentY = y;
                    destinationFound = true;
                    return makePath(grid, _targetNode);
                }
                else if (closedSet[grid.maze[x][y]->fourNeighbors[i]->x][grid.maze[x][y]->fourNeighbors[i]->y] == false)
                {
                    gNew = currentNode->gCost + 1.0;
                    hNew = calculateH(grid.maze[x][y]->fourNeighbors[i]->x, grid.maze[x][y]->fourNeighbors[i]->y, _targetNode);
                    fNew = gNew + hNew;

                    // Check if this path is better than the one already present
                    if (grid.maze[grid.maze[x][y]->fourNeighbors[i]->x][grid.maze[x][y]->fourNeighbors[i]->y]->fCost == FLT_MAX || grid.maze[grid.maze[x][y]->fourNeighbors[i]->x][grid.maze[x][y]->fourNeighbors[i]->y]->fCost > fNew)
                    {
                        // Update the details of this neighbour node
                        grid.maze[grid.maze[x][y]->fourNeighbors[i]->x][grid.maze[x][y]->fourNeighbors[i]->y]->fCost = fNew;
                        grid.maze[grid.maze[x][y]->fourNeighbors[i]->x][grid.maze[x][y]->fourNeighbors[i]->y]->gCost = gNew;
                        grid.maze[grid.maze[x][y]->fourNeighbors[i]->x][grid.maze[x][y]->fourNeighbors[i]->y]->hCost = hNew;
                        grid.maze[grid.maze[x][y]->fourNeighbors[i]->x][grid.maze[x][y]->fourNeighbors[i]->y]->parentX = x;
                        grid.maze[grid.maze[x][y]->fourNeighbors[i]->x][grid.maze[x][y]->fourNeighbors[i]->y]->parentY = y;
                        openSet.emplace_back(grid.maze[grid.maze[x][y]->fourNeighbors[i]->x][grid.maze[x][y]->fourNeighbors[i]->y]);
                    }
                }
            }
        }









        //CHECK EIGHT NEIGHBORS
        //for (int newX = -1; newX <= 1; newX++) {
        //    for (int newY = -1; newY <= 1; newY++) {
        //        double gNew, hNew, fNew;
        //        if (isValid(x + newX, y + newY ,grid)) {
        //            if (isDestination(x + newX, y + newY, _targetNode))
        //            {
        //                //Destination found - make path
        //                grid.maze[x + newX][y + newY]->parentX = x;
        //                grid.maze[x + newX][y + newY]->parentY = y;
        //                destinationFound = true;
        //                return makePath(grid,_targetNode);
        //            }
        //            else if (closedSet[x + newX][y + newY] == false)
        //            {
        //                gNew = currentNode->gCost + 1.0;
        //                hNew = calculateH(x + newX, y + newY, _targetNode);
        //                fNew = gNew + hNew;

        //                // Check if this path is better than the one already present
        //                if (grid.maze[x + newX][y + newY]->fCost == FLT_MAX || grid.maze[x + newX][y + newY]->fCost > fNew)
        //                {
        //                    // Update the details of this neighbour node
        //                    grid.maze[x + newX][y + newY]->fCost = fNew;
        //                    grid.maze[x + newX][y + newY]->gCost = gNew;
        //                    grid.maze[x + newX][y + newY]->hCost = hNew;
        //                    grid.maze[x + newX][y + newY]->parentX = x;
        //                    grid.maze[x + newX][y + newY]->parentY = y;
        //                    openSet.emplace_back(grid.maze[x + newX][y + newY]);
        //                }
        //            }
        //        }
        //    }
        //}


    }
    if (destinationFound == false) {
        cout << "Destination not found" << endl;
        return empty;
    }	

    return empty;
}




bool Pathfinding::isValid(int x, int y, Grid& grid) { //If our Grid::Node is an obstacle it is not valid
    int id = x + y * (X_MAX / X_STEP); 
    
    if (x < 0 || y < 0 || x >= (X_MAX / X_STEP) || y >= (Y_MAX / Y_STEP)) { //Check max distance to walk
        return false;
    }
    else
    {
        if (grid.maze[x][y]->isWay) { //check if is way
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}


bool Pathfinding::isDestination(int x, int y, Grid::Node dest) {
    if (x == dest.x && y == dest.y) {
        return true;
    }
    return false;
}


double Pathfinding::calculateH(int x, int y, Grid::Node dest) {
   // double H = (sqrt((x - dest.x) * (x - dest.x) + (y - dest.y) * (y - dest.y)));

   //double H = (dest.x - x) + (dest.y - y);
   double H = abs(dest.x - x) + abs(dest.y - y);
    
    //double dstX = abs(x - dest.x);
    //double dstY = abs(y - dest.y);
    //if (dstX > dstY)
    //{
    //    return 14 * dstY + 10 * (dstX - dstY);
    //}
    //else
    //{
    //    return 14 * dstX + 10 * (dstY - dstX);
    //}

    return H;
}

vector<Grid::Node*> Pathfinding::makePath(Grid& grid,Grid::Node dest) {
    try {
        cout << "Found a path" << endl;
        int x = dest.x;
        int y = dest.y;
        stack<Grid::Node*> path;
        vector<Grid::Node*> usablePath;

        while (!(grid.maze[x][y]->parentX == x && grid.maze[x][y]->parentY == y) && grid.maze[x][y]->x != -1 && grid.maze[x][y]->y != -1)
        {
            path.push(grid.maze[x][y]);
            int tempX = grid.maze[x][y]->parentX;
            int tempY = grid.maze[x][y]->parentY;
            x = tempX;
            y = tempY;
        }
        path.push(grid.maze[x][y]);

        while (!path.empty()) {
            Grid::Node* top = path.top();
            path.pop();
            usablePath.emplace_back(top);
        }
        return usablePath;
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
}