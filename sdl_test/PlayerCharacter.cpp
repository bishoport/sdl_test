#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include <algorithm>


PlayerCharacter::PlayerCharacter(){}

PlayerCharacter::PlayerCharacter(const char* _filename, Grid& _grid)
{
	grid = _grid;
	SDL_Surface* tmpSurface = IMG_Load(_filename);
	characterTex = SDL_CreateTextureFromSurface(getRenderer(), tmpSurface);
	SDL_FreeSurface(tmpSurface);

	transform.position.x = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x + 25;
	transform.position.y = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y;
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

				endX = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x + 25;
				endY = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y;

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
		/*transform.position.x = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.x + 25;
		transform.position.y = grid.maze[currentGridPosition.x][currentGridPosition.y]->centerPoint.y;*/
	}

}



void PlayerCharacter::draw()
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
