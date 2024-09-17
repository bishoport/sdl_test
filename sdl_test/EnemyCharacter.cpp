#include "EnemyCharacter.h"

#include <thread>
#include <SDL.h>
#include <chrono>
#include <mutex>


EnemyCharacter::EnemyCharacter(){}


EnemyCharacter::EnemyCharacter(const char* _filename, Grid* _grid)
{
	pathfinding = Pathfinding();
	grid = _grid;

	EnemyCharacter::loadMedia(_filename);
}


EnemyCharacter::~EnemyCharacter()
{
	cout << "ME destruyen!!!" << endl;
	gSpriteSheetTexture.free();
}

bool EnemyCharacter::loadMedia(const char* _filename)
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if (!gSpriteSheetTexture.loadFromFile(_filename))
	{
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	}
	else
	{
		//Set UP sprite
		gSpriteClips[0].x = Grid::SIZE_NODE * 0;  
		gSpriteClips[0].y = Grid::SIZE_NODE * 0;
		gSpriteClips[0].w = Grid::SIZE_NODE;
		gSpriteClips[0].h = Grid::SIZE_NODE;

		////Set DOWN sprite
		//gSpriteClips[1].x = Grid::SIZE_NODE * 1;
		//gSpriteClips[1].y = Grid::SIZE_NODE * 0;
		//gSpriteClips[1].w = Grid::SIZE_NODE;
		//gSpriteClips[1].h = Grid::SIZE_NODE;

		////Set RIGHT sprite
		//gSpriteClips[2].x = Grid::SIZE_NODE * 2;
		//gSpriteClips[2].y = Grid::SIZE_NODE * 0;
		//gSpriteClips[2].w = Grid::SIZE_NODE;
		//gSpriteClips[2].h = Grid::SIZE_NODE;

		////Set LEFT sprite
		//gSpriteClips[3].x = Grid::SIZE_NODE * 3;
		//gSpriteClips[3].y = Grid::SIZE_NODE * 0;
		//gSpriteClips[3].w = Grid::SIZE_NODE;
		//gSpriteClips[3].h = Grid::SIZE_NODE;

		//Set damage sprite
		gSpriteClips[1].x = Grid::SIZE_NODE * 5;
		gSpriteClips[1].y = Grid::SIZE_NODE * 5;
		gSpriteClips[1].w = Grid::SIZE_NODE;
		gSpriteClips[1].h = Grid::SIZE_NODE;
	}

	return success;
}


void EnemyCharacter::move()
{
	isPlayerInAgro = false;

	if (this->isDead || this->isInDamage || this->isMoving) return;

	for (int i = 0; i < grid->maze[currentGridPosition.x][currentGridPosition.y]->fourNeighbors.size(); i++)
	{
		if (grid->maze[currentGridPosition.x][currentGridPosition.y]->fourNeighbors[i]->isPlayerNode)
		{
			ReferencesManager::getInstance()->getPlayerCharacter()->DoDamage(1);
			return;
		}
	}

	path.clear();
	path = pathfinding.FindPath(*grid->maze[currentGridPosition.x][currentGridPosition.y], *grid->maze[ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.x][ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.y], *grid);

	for (int i = 1; i < path.size(); i++)
	{
		path[i]->isActive = true;
	}

	if (path.size() > 0)
	{
		int NodeToPosition = 0;

		if (path[1]->isPlayerNode)
		{
			NodeToPosition = 0;
		}
		else
		{
			NodeToPosition = 1;
			isPlayerInAgro = false;
		}

		path[0]->enemyInNode = nullptr;
		path[0]->isEnemyNode = false;

		currentGridPosition.x = path[NodeToPosition]->x;
		currentGridPosition.y = path[NodeToPosition]->y;

		path[NodeToPosition]->enemyInNode = this;
		path[NodeToPosition]->isEnemyNode = true;

		startX = transform.position.x;
		startY = transform.position.y;

		endX = grid->maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x - Grid::SIZE_NODE / 2;
		endY = grid->maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y - Grid::SIZE_NODE / 2;

		isMoving = true;
	}

	
	

}






void EnemyCharacter::update(float deltaTime)
{
	//TIME UPDATE IN BASE
	Character::update(deltaTime); 

	if (isMoving)
	{
		tick += deltaTime;
		float tval = min(tick / duration, 1.0f);

		float myX = startX + (endX - startX) * tval;
		float myY = startY + (endY - startY) * tval;

		if (tval == 1)
		{
			//finished transition
			isMoving = false;
			tick = 0.0f;
			grid->unActiveAllNodes();
		}

		transform.position.x = myX;
		transform.position.y = myY;
	}
	else
	{
		transform.position.x = grid->maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x - Grid::SIZE_NODE / 2;
		transform.position.y = grid->maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y - Grid::SIZE_NODE / 2;
	}
}



void EnemyCharacter::draw()
{
	if (grid->maze[currentGridPosition.x][currentGridPosition.y]->isDiscovered)
	{
		gSpriteSheetTexture.render(transform.position.x, transform.position.y, SDL_FLIP_NONE, &gSpriteClips[0]);

		if (isInDamage == true)
		{
			gSpriteSheetTexture.render(transform.position.x, transform.position.y, SDL_FLIP_NONE, &gSpriteClips[1]);
		}
	}
}








void EnemyCharacter::DoDamage(int value)
{
	if (this->isDead == true) 
	{
		cout << &EnemyCharacter::isDead << endl;
		return;
	}
	this->lifePoint -= value;
	//cout << this->lifePoint << endl;

	if (this->lifePoint <= 0)
	{
		this->isDead = true;

		grid->getNodeByCoordinates(currentGridPosition.x, currentGridPosition.y)->enemyInNode = nullptr;
		grid->getNodeByCoordinates(currentGridPosition.x, currentGridPosition.y)->isEnemyNode = false;
	}
	else
	{
		isInDamage = true;
		timerPauseCoroutine->StartPause([&]() { //Con & le pasamos el parametro de this dentro de la expresion lambda
				(*this).isInDamage = false;
		}, 10);
	}
}





