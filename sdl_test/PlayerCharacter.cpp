#include "PlayerCharacter.h"
#include <SDL_image.h>

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

	cout << axis << endl;

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
		cout << "Está el enemigo" << endl;
		//grid.getNodeByCoordinates(checkX, checkY)->characterInNode->DoDamage(10);
	}
	else
	{
		if (grid.getNodeByCoordinates(checkX, checkY)->isWay)
		{
			currentGridPosition.x = checkX;
			currentGridPosition.y = checkY;
		}
		else
		{
			cout << "Muralla" << endl;
		}
	}
}
