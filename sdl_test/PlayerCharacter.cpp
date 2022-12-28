#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include <algorithm>


PlayerCharacter::PlayerCharacter(){}

PlayerCharacter::~PlayerCharacter()
{
	gSpriteSheetTexture.free();
}

PlayerCharacter::PlayerCharacter(const char* _filename, Grid& _grid)
{
	grid = _grid;

	PlayerCharacter::loadMedia(_filename);

	transform.position.x = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x + Grid::SIZE_NODE / 2;
	transform.position.y = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y + Grid::SIZE_NODE / 2;
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
		//Set idle sprite
		gSpriteClips[0].x = Grid::SIZE_NODE * 3;
		gSpriteClips[0].y = Grid::SIZE_NODE * 4;
		gSpriteClips[0].w = Grid::SIZE_NODE;
		gSpriteClips[0].h = Grid::SIZE_NODE;
	}

	return success;
}



void PlayerCharacter::Move(const char* axis)
{
	if (isMoving) return;

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
		if (controlCooldown == 0) {
			((EnemyCharacter*)grid.getNodeByCoordinates(checkX, checkY)->enemyInNode)->DoDamage(10);
			controlCooldown = 5;
		}
	}
	else
	{
		if (isMoving == false)
		{
			if (grid.getNodeByCoordinates(checkX, checkY)->isWay)
			{
				grid.maze[currentGridPosition.x][currentGridPosition.y]->isPlayerNode = false;
				currentGridPosition.x = checkX;
				currentGridPosition.y = checkY;
				grid.maze[currentGridPosition.x][currentGridPosition.y]->isPlayerNode = true;

				startX = transform.position.x;
				startY = transform.position.y;

				endX = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x + Grid::SIZE_NODE / 2;
				endY = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y + Grid::SIZE_NODE / 2;

				isMoving = true;
			}
			else
			{
				//cout << "Muralla" << endl;
			}
		}
	}
}




void PlayerCharacter::update(float deltaTime)
{
	//this little if makes your cooldown go back to zero slowly
	if (controlCooldown > 0) {
		controlCooldown--;
		//cout << "controlCooldown " << controlCooldown << endl;
	}

	

	//transform.position.x = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x + 25;
	//transform.position.y = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y;

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
			//cout << "ACABA LA TRANSICION " << myX << endl;
		}
		//cout << "myX " << myX << endl;
		//cout << "myY " << myY << endl;

		transform.position.x = myX;
		transform.position.y = myY;
		
	}
	else
	{
		transform.position.x = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x + Grid::SIZE_NODE / 2;
		transform.position.y = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y + Grid::SIZE_NODE / 2;
	}

}



void PlayerCharacter::draw()
{
	gSpriteSheetTexture.render(transform.position.x, transform.position.y, &gSpriteClips[0]);
}
