#include "EnemyCharacter.h"

#include <thread>
#include <SDL.h>
#include <SDL_thread.h>
#include <chrono>
#include <mutex>


typedef struct {
	EnemyCharacter *enemy;
} ThreadData;



int threadFunction(void* data)
{
	//cout << "EMPIEZA" << endl;
	static std::mutex m;

	ThreadData* tdata = (ThreadData*)data;
	EnemyCharacter* enemy = tdata->enemy;

	Uint32 totalFrameTicks = 0;
	Uint32 totalFrames = 0;

	Uint32 startTicks = SDL_GetTicks();
	Uint64 startPerf = SDL_GetPerformanceCounter();

	enemy->isInDamage = true;

	m.lock();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	//cout << "ACABA" << endl;
	enemy->isInDamage = false;

	free(data);
	m.unlock();
	return 0;
}





EnemyCharacter::EnemyCharacter()
{
	
}

EnemyCharacter::~EnemyCharacter() 
{
	cout << "ME destruyen!!!" << endl;
	gSpriteSheetTexture.free();
}

EnemyCharacter::EnemyCharacter(const char* _filename, Grid& _grid)
{
	pathfinding = Pathfinding();
	grid = _grid;

	EnemyCharacter::loadMedia(_filename);
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
		//Set idle sprite
		gSpriteClips[0].x = Grid::SIZE_NODE * 9;
		gSpriteClips[0].y = Grid::SIZE_NODE * 3;
		gSpriteClips[0].w = Grid::SIZE_NODE;
		gSpriteClips[0].h = Grid::SIZE_NODE;
	}

	return success;
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
			//cout << "Está el player" << endl;
			NodeToPosition = 0;
		}
		else
		{
			NodeToPosition = 1;
		}


		path[0]->enemyInNode = nullptr;
		path[0]->isEnemyNode = false;
		currentGridPosition.x = path[NodeToPosition]->x;
		currentGridPosition.y = path[NodeToPosition]->y;
		path[NodeToPosition]->enemyInNode = this;
		path[NodeToPosition]->isEnemyNode = true;

		startX = transform.position.x;
		startY = transform.position.y;

		endX = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x + Grid::SIZE_NODE / 2;
		endY = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y + Grid::SIZE_NODE / 2;

		isMoving = true;
	}
}






void EnemyCharacter::update(float deltaTime)
{
	if (isMoving)
	{
		tick += deltaTime;
		//cout << "tick " << tick << endl;
		float tval = min(tick / duration, 1.0f);

		float myX = startX + (endX - startX) * tval;
		float myY = startY + (endY - startY) * tval;

		if (tval == 1)
		{
			//finished transition
			isMoving = false;
			tick = 0.0f;
			grid.unActiveAllNodes();
			//cout << "ACABA LA TRANSICION " << myX << endl;
		}
		/*cout << "myX " << myX << endl;
		cout << "myY " << myY << endl;*/

		transform.position.x = myX;
		transform.position.y = myY;

	}
	else
	{
		transform.position.x = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x + Grid::SIZE_NODE / 2;
		transform.position.y = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y + Grid::SIZE_NODE / 2;
	}
}



void EnemyCharacter::draw()
{
	gSpriteSheetTexture.render(transform.position.x, transform.position.y, &gSpriteClips[0]);
}





void EnemyCharacter::DoDamage(int value)
{
	if (this == NULL) return;

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

		grid.getNodeByCoordinates(currentGridPosition.x, currentGridPosition.y)->enemyInNode = nullptr;
		grid.getNodeByCoordinates(currentGridPosition.x, currentGridPosition.y)->isEnemyNode = false;

		ReferencesManager::getInstance()->enemies.erase(std::remove(ReferencesManager::getInstance()
									    ->enemies.begin(), ReferencesManager::getInstance()->enemies.end(), 
										this), ReferencesManager::getInstance()->enemies.end());
	}
	else
	{
	//	int data = 101;
		ThreadData* data = new ThreadData();
		data->enemy = this;

		SDL_Thread* threadID = SDL_CreateThread(threadFunction, "LazyThread", (void*)data);

	//	cout << "VAMOS" << endl;
	}

}


