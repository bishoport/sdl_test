#include "Character.h"

using namespace std;


void Character::displayCharacter()
{
	
	transform.position.x = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x + 25;
	transform.position.y = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y;

	characterRect.h = transform.scale.y;
	characterRect.w = transform.scale.x;
	characterRect.x = transform.position.x;
	characterRect.y = transform.position.y;

	SDL_RenderCopy(getRenderer(), characterTex, NULL, &characterRect);
}


void Character::SetGridPosition(Vector2 gridposition)
{
	
	currentGridPosition = gridposition;

	transform.position.x = grid.maze[gridposition.x][gridposition.y]->centerPoint.x + 25;
	transform.position.y = grid.maze[gridposition.x][gridposition.y]->centerPoint.y;
}
