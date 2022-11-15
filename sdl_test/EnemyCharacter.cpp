#include "EnemyCharacter.h"


EnemyCharacter::EnemyCharacter()
{
	
}

EnemyCharacter::~EnemyCharacter() 
{
	cout << "ME destruyen!!!" << endl;
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
	if (this->isDead) return;

	path.clear();
	path = pathfinding.FindPath(*grid.maze[currentGridPosition.x][currentGridPosition.y],
		*grid.maze[ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.x][ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.y], grid);

	for (int i = 1; i < path.size(); i++)
	{
		path[i]->isActive = true;
	}

	grid.maze[ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.x][ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.y]->isPlayerNode = true;
	//grid.maze[currentGridPosition.x][currentGridPosition.y]->isEnemyNode = true;

	if (path.size() > 0)
	{
		int NodeToPosition = 0;
		if (path[1]->isPlayerNode)
		{
			cout << "Está el player" << endl;
			NodeToPosition = 0;
		}
		else
		{
			NodeToPosition = 1;
		}

		path[NodeToPosition]->enemyInNode = this;

		currentGridPosition.x = path[NodeToPosition]->x;
		currentGridPosition.y = path[NodeToPosition]->y;
		grid.getNodeByCoordinates(currentGridPosition.x, currentGridPosition.y)->enemyInNode = this;
		path[NodeToPosition]->isEnemyNode = true;
	}
}



void EnemyCharacter::DoDamage(int value)
{
	if (this->isDead == true) {
		cout << &EnemyCharacter::isDead << endl;
		return;
	}
	this->lifePoint -= value;
	//cout << this->lifePoint << endl;

	if (this->lifePoint <= 0)
	{
		cout << "character is dead" << endl;
		this->isDead = true;
	}
}
