#include "EnemyCharacter.h"
#include "ReferencesManager.h"

EnemyCharacter::EnemyCharacter()
{

}

EnemyCharacter::EnemyCharacter(const char* _filename, Grid& _grid)
{

	pathfinding = Pathfinding();
	grid = _grid;
	SDL_Surface* tmpSurface = IMG_Load(_filename);
	characterTex = SDL_CreateTextureFromSurface(getRenderer(), tmpSurface);
	SDL_FreeSurface(tmpSurface);
}

void EnemyCharacter::Move()
{
	path.clear();
	path = pathfinding.FindPath(*grid.maze[currentGridPosition.x][currentGridPosition.y],
		*grid.maze[ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.x][ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.y], grid);

	for (int i = 1; i < path.size(); i++)
	{
		path[i]->isActive = true;
	}

	grid.maze[ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.x][ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.y]->isPlayerNode = true;
	//grid.maze[currentGridPosition.x][currentGridPosition.y]->isEnemyNode = true;

	if (path.size() > 1)
	{
		if (path[1]->isPlayerNode)
		{
			cout << "Está el player" << endl;
		}
		else
		{
			//path[1]->enemyInNode = this;
			currentGridPosition.x = path[1]->x;
			currentGridPosition.y = path[1]->y;
			path[1]->isEnemyNode = true;
		}
	}
}
