#include "Character.h"

using namespace std;


void Character::draw()
{

}

void Character::update(float deltaTime)
{

}


void Character::SetGridPosition(Vector2 gridposition)
{
	currentGridPosition = gridposition;
	transform.position.x = grid.maze[gridposition.x][gridposition.y]->centerPoint.x + 25;
	transform.position.y = grid.maze[gridposition.x][gridposition.y]->centerPoint.y;
}
