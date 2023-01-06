#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include <algorithm>


TimerPause* timerPauseCoroutinePlayer;

PlayerCharacter::PlayerCharacter()
{
	timerPauseCoroutinePlayer = new TimerPause();
}

PlayerCharacter::~PlayerCharacter()
{
	gSpriteSheetTexture.free();
}

PlayerCharacter::PlayerCharacter(const char* _filename, Grid* _grid)
{
	timerPauseCoroutinePlayer = new TimerPause();

	grid = _grid;

	PlayerCharacter::loadMedia(_filename);

	transform.position.x = grid->maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x - Grid::SIZE_NODE / 2;
	transform.position.y = grid->maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y - Grid::SIZE_NODE / 2;
}


bool PlayerCharacter::loadMedia(const char* _filename)
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

		//Set DOWN sprite
		gSpriteClips[1].x = Grid::SIZE_NODE * 1;
		gSpriteClips[1].y = Grid::SIZE_NODE * 0;
		gSpriteClips[1].w = Grid::SIZE_NODE;
		gSpriteClips[1].h = Grid::SIZE_NODE;

		//Set RIGHT sprite
		gSpriteClips[2].x = Grid::SIZE_NODE * 2;
		gSpriteClips[2].y = Grid::SIZE_NODE * 0;
		gSpriteClips[2].w = Grid::SIZE_NODE;
		gSpriteClips[2].h = Grid::SIZE_NODE;

		//Set LEFT sprite
		gSpriteClips[3].x = Grid::SIZE_NODE * 3;
		gSpriteClips[3].y = Grid::SIZE_NODE * 0;
		gSpriteClips[3].w = Grid::SIZE_NODE;
		gSpriteClips[3].h = Grid::SIZE_NODE;

		//Set damage sprite
		/*gSpriteClips[1].x = Grid::SIZE_NODE * 5;
		gSpriteClips[1].y = Grid::SIZE_NODE * 5;
		gSpriteClips[1].w = Grid::SIZE_NODE;
		gSpriteClips[1].h = Grid::SIZE_NODE;*/
	}

	return success;
}



void PlayerCharacter::MoveFromInput()
{
	if (isMoving || isInDamage) return;

	grid->unActiveAllNodes();

	int checkX = currentGridPosition.x;
	int checkY = currentGridPosition.y;

	//cout << axis << endl;

	if (firstActionSelected == "-Y")
	{
		checkY -= 1;
	}
	else if (firstActionSelected == "+Y")
	{
		checkY += 1;
	}
	else if (firstActionSelected == "-X")
	{
		checkX -= 1;
	}
	else if (firstActionSelected == "+X")
	{
		checkX += 1;
	}

	//-1 -> None
	// 0 -> Only Move DOWN
	// 1 -> near attack RIGHT
	// 2 -> far attack UP
	// 3 -> with shiled LEFT

	cout << "secondActionSelected -> " << secondActionSelected << endl;
	if (grid->getNodeByCoordinates(checkX, checkY)->isEnemyNode)
	{
		if(secondActionSelected == 1)
		{ 
			cout << "ESPADA PUESTA" << endl;
			((EnemyCharacter*)grid->getNodeByCoordinates(checkX, checkY)->enemyInNode)->DoDamage(10);
			
			timerPauseCoroutine->StartPause([&]() { //Con & le pasamos el parametro de this dentro de la expresion lambda
				(*this).secondActionSelected = -1;
			}, 20);
		}
	}
	else if (grid->getNodeByCoordinates(checkX, checkY)->isDoor)
	{
		grid->getNodeByCoordinates(checkX, checkY)->isDoor = false;
		grid->getNodeByCoordinates(checkX, checkY)->isWay = true;
	}
	else
	{
		if (isMoving == false)
		{
			if (grid->getNodeByCoordinates(checkX, checkY)->isWay)
			{
				grid->maze[currentGridPosition.x][currentGridPosition.y]->isPlayerNode = false;
				currentGridPosition.x = checkX;
				currentGridPosition.y = checkY;

				startX = transform.position.x;
				startY = transform.position.y;

				endX = grid->maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x - Grid::SIZE_NODE / 2;
				endY = grid->maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y - Grid::SIZE_NODE / 2;

				isMoving = true;
			}
			else
			{
				//cout << "Muralla" << endl;
			}
		}
	}

	
}


void PlayerCharacter::DoDamage(int value)
{
	if (this->isDead == true) {
		cout << &PlayerCharacter::isDead << endl;
		return;
	}

	if (secondActionSelected == 3)
	{
		cout << "SHIELD PUESTO" << endl;
	}
	else
	{
		this->lifePoint -= value;
		cout << this->lifePoint << endl;
		if (this->lifePoint <= 0)
		{
			this->isDead = true;
			grid->getNodeByCoordinates(currentGridPosition.x, currentGridPosition.y)->isPlayerNode = false;
		}
		else
		{
			isInDamage = true;
			timerPauseCoroutine->StartPause([&]() { //Con & le pasamos el parametro de this dentro de la expresion lambda
				(*this).isInDamage = false;
				}, 10);
		}
	}

	
}




void PlayerCharacter::move()
{
}

void PlayerCharacter::update(float deltaTime)
{

	//TIME UPDATE IN BASE
	Character::update(deltaTime);


	if (isMoving )
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

			grid->setDiscoveredByRange(currentGridPosition.y, currentGridPosition.x,1);

			grid->maze[currentGridPosition.x][currentGridPosition.y]->isPlayerNode = true;

			for (int i = 0; i < grid->maze[currentGridPosition.x][currentGridPosition.y]->eightNeighbors.size(); i++)
			{
				grid->maze[currentGridPosition.x][currentGridPosition.y]->eightNeighbors[i]->isActive = true;
			}
			
			//cout << "ACABA LA TRANSICION " << myX << endl;
		}

		transform.position.x = myX;
		transform.position.y = myY;
	}
	//else
	//{
		/*transform.position.x = grid->maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x;
		transform.position.y = grid->maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y;*/
	//}

}



void PlayerCharacter::draw()
{

	SDL_RendererFlip flipType = SDL_FLIP_NONE;

	int spriteByDirection = 0;

	if (firstActionSelected == "-Y") //Mirando Izquierda
	{
		spriteByDirection = 3;
		
	}
	else if (firstActionSelected == "+Y") //Mirando Derecha
	{
		spriteByDirection = 2;
	}
	else if (firstActionSelected == "-X") //Mirando Arriba
	{
		spriteByDirection = 0;

	}
	else if (firstActionSelected == "+X") //Mirando Abajo
	{
		spriteByDirection = 1;
	}

	if (isDead == false)
	{

		gSpriteSheetTexture.render(transform.position.x, transform.position.y, flipType, &gSpriteClips[spriteByDirection]);

		//if (secondActionSelected == 3)
		//{
		//	//gSpriteSheetTexture.render(transform.position.x, transform.position.y, flipType, &gSpriteClips[spriteByDirection]);
		//}
		//else
		//{
		//	gSpriteSheetTexture.render(transform.position.x, transform.position.y, flipType, &gSpriteClips[spriteByDirection]);

		//	if (isInDamage == true)
		//	{
		//		gSpriteSheetTexture.render(transform.position.x, transform.position.y, flipType, &gSpriteClips[1]);
		//	}
		//}
	}
}
