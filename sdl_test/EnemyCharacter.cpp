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
}

EnemyCharacter::EnemyCharacter(const char* _IdleImage, const char* _DamageImage, Grid& _grid)
{
	pathfinding = Pathfinding();
	grid = _grid;
	SDL_Surface* tmpSurface = IMG_Load(_IdleImage);
	characterTex = SDL_CreateTextureFromSurface(getRenderer(), tmpSurface);

	tmpSurface = IMG_Load(_DamageImage);
	characterDamageTex = SDL_CreateTextureFromSurface(getRenderer(), tmpSurface);

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

		endX = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x + 25;
		endY = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y;

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
}



void EnemyCharacter::draw()
{


	characterRect.h = transform.scale.y;
	characterRect.w = transform.scale.x;
	characterRect.x = transform.position.x;
	characterRect.y = transform.position.y;

	SDL_RenderCopy(getRenderer(), characterTex, NULL, &characterRect);

	if (isInDamage == false)
	{
		SDL_RenderCopy(getRenderer(), characterTex, NULL, &characterRect);
	}
	else
	{
		SDL_RenderCopy(getRenderer(), characterDamageTex, NULL, &characterRect);
	}
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


