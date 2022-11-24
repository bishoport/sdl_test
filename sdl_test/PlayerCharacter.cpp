#include "PlayerCharacter.h"
#include "EnemyCharacter.h"


PlayerCharacter::PlayerCharacter()
{

}

PlayerCharacter::PlayerCharacter(const char* _filename, Grid& _grid)
{
	grid = _grid;
	SDL_Surface* tmpSurface = IMG_Load(_filename);
	characterTex = SDL_CreateTextureFromSurface(getRenderer(), tmpSurface);
	SDL_FreeSurface(tmpSurface);
}


void PlayerCharacter::Move(const char* axis)
{
	int checkX = currentGridPosition.x;
	int checkY = currentGridPosition.y;

	//cout << axis << endl;

	if (axis == "-Y")
	{
		checkY -= 1;
	}
	else if (axis == "+Y")
	{
		checkY += 1;
	}
	else if (axis == "-X")
	{
		checkX -= 1;
	}
	else if (axis == "+X")
	{
		checkX += 1;
	}

	if (grid.getNodeByCoordinates(checkX, checkY)->isEnemyNode)
	{
		//cout << "Está el enemigo" << endl;
		//Casting de void* en Node a EnemyCharacter
		((EnemyCharacter*)grid.getNodeByCoordinates(checkX, checkY)->enemyInNode)->DoDamage(1);

	}
	else
	{
		if (grid.getNodeByCoordinates(checkX, checkY)->isWay)
		{
			grid.maze[currentGridPosition.x][currentGridPosition.y]->isPlayerNode = false;
			currentGridPosition.x = checkX;
			currentGridPosition.y = checkY;
			grid.maze[currentGridPosition.x][currentGridPosition.y]->isPlayerNode = true;
		}
		else
		{
			//cout << "Muralla" << endl;
		}
	}
}
