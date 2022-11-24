#include "EnemyCharacter.h"



#include <thread>
#include <ctime> // time header
#include <SDL.h>
#include <SDL_thread.h>

int threadFunction(void* data)
{
	cout << "EMPIEZA" << endl;
	//cout << "Please enter seconds: ";//prompt for user
	//int timer = 5; //int for user input
	//cin >> timer; //user input
	int timer = clock();
	while ((timer / 1000) < 5) // While the program hasn't been going for 60 second
	{
		//system("cls"); // Clear All of the text
		timer = clock();  // Update the timer var
		cout << "Seconds since started: " << timer / 1000 << endl; // Print the time since the program started in seconds
		break;
	}
	//cout << "The program has been going for a minute" << endl;
	//system("pause");
	cout << "ACABA" << endl;

	return 0;
}





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
		int data = 101;
		SDL_Thread* threadID = SDL_CreateThread(threadFunction, "LazyThread", (void*)data);
		cout << "VAMOS" << endl;
		//SDL_WaitThread(threadID, NULL);
		//SDL_DetachThread(threadID);
		//SDLK_thread
	}

}


