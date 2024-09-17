#include "Character.h"

using namespace std;


void Character::init()
{
	timerPauseCoroutine = new TimerPause();
}

void Character::draw()
{

}

void Character::move()
{
}

void Character::update(float deltaTime)
{
	timerPauseCoroutine->update(deltaTime);
}


void Character::SetGridPosition(Vector2 gridposition)
{
	currentGridPosition = gridposition;
	transform.position.x = grid->maze[gridposition.x][gridposition.y]->centerPoint.x - Grid::SIZE_NODE / 2;
	transform.position.y = grid->maze[gridposition.x][gridposition.y]->centerPoint.y - Grid::SIZE_NODE / 2;
}
