#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "initclose.h"
#include "renderer.h"



#include "Grid.h"
#include "Pathfinding.h"
#include "Character.h"
#include <math.h>
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "ReferencesManager.h"


using namespace std;


typedef struct gameT
{
	SDL_Event event;
	int loopDone;
	SDL_Rect mouseRect;
	Vector2 mousePoint;
	Vector2 mapScroll2Dpos;
	int mapScrolllSpeed;
	int lastTileClicked;
}gameT;

gameT game;

Grid grid;





ReferencesManager* ReferencesManager::_instance;


void init()
{
	grid = Grid();

	game.loopDone = 0;
	SDL_PumpEvents();  // make sure we have the latest mouse state

	//Player
	PlayerCharacter* player;
	player = new PlayerCharacter( "assets/sprites/player.png",grid);
	player->SetGridPosition({3,5});
	player->transform.scale.x = 50;
	player->transform.scale.y = 50;
	ReferencesManager::getInstance()->setPlayerCharacter(player);

	for (int i = 0; i < 1; i++)
	{
		//Enemy
		EnemyCharacter* enemy;
		enemy = new EnemyCharacter("assets/sprites/Alien.png", grid);
		enemy->SetGridPosition({ 6,1+i });
		enemy->transform.scale.x = 50;
		enemy->transform.scale.y = 50;

		ReferencesManager::getInstance()->enemies.push_back(enemy);
	}
}


void draw()
{
	SDL_SetRenderDrawColor(getRenderer(), 0x3b, 0x3b, 0x3b, 0x00);
	SDL_RenderClear(getRenderer());

	//DRAW STUFFS
	grid.displayMaze();

	ReferencesManager::getInstance()->getPlayerCharacter()->displayCharacter();
	grid.maze[ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.x][ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.y]->isPlayerNode = true;

	for (int i = 0; i < ReferencesManager::getInstance()->enemies.size(); i++)
	{
		if (dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[i]))
		{
			EnemyCharacter* enemy = dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[i]);

			if (enemy->isDead == false)
			{
				ReferencesManager::getInstance()->enemies[i]->displayCharacter();
				grid.maze[enemy->currentGridPosition.x][enemy->currentGridPosition.y]->isEnemyNode = true;
			}
		}
	}

	SDL_RenderPresent(getRenderer());
	//Don't be a CPU HOG!! :D
	SDL_Delay(10);
}

void update()
{
	SDL_GetMouseState(&game.mouseRect.x, &game.mouseRect.y);
}

void moveAllEnemies()
{
	for (int i = 0; i < ReferencesManager::getInstance()->enemies.size(); i++)
	{
		if (dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[i]))
		{
			EnemyCharacter* enemy = dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[i]);

			if (enemy->isDead == false)
			{
				enemy->Move();
			}
		}
	}
}



void updateInput()
{
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&game.event) != 0)
	{
		switch (game.event.type)
		{
		case SDL_QUIT:
			game.loopDone = 1;
			break;

		case SDL_KEYDOWN:
			switch (game.event.key.keysym.sym) {
			case SDLK_a:

				ReferencesManager::getInstance()->getPlayerCharacter()->Move("-Y");
				moveAllEnemies();
				break;

			case SDLK_d:
				ReferencesManager::getInstance()->getPlayerCharacter()->Move("+Y");
				moveAllEnemies();
				break;

			case SDLK_w:
				ReferencesManager::getInstance()->getPlayerCharacter()->Move("-X");
				moveAllEnemies();
				break;

			case SDLK_s:
				ReferencesManager::getInstance()->getPlayerCharacter()->Move("+X");
				moveAllEnemies();
				break;
			default:break;
			}
			break;

		case SDL_KEYUP:
			switch (game.event.key.keysym.sym) {
			case SDLK_ESCAPE:
				game.loopDone = 1;
				break;
			default:break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:

			if (game.event.button.button == SDL_BUTTON_LEFT)
			{
				//enemies.erase(std::remove(enemies.begin(), enemies.end(), enemies[0]), enemies.end());

				//int x, y;
				////Uint32 buttons;
				//SDL_GetMouseState(&x, &y);
				////
				//int selected_X = (float)y / (float) Grid::SIZE_NODE;
				//int selected_Y = (float)x / (float) Grid::SIZE_NODE;

				//Grid::Node* n = grid.getNodeByCoordinates(selected_X, selected_Y);
				//n->isActive = true;

				

				//for (int i = 0; i < grid.maze[selected_X][selected_Y]->eightNeighbors.size(); i++)
				//{
					//grid.maze[selected_X][selected_Y]->fourNeighbors[i]->isActive = true;
					//grid.maze[selected_X][selected_Y]->eightNeighbors[i]->isActive = true;
				//}

				/*Pathfinding pthFTemp = Pathfinding();
				vector<Grid::Node*> path;
				path.clear();

				path = pthFTemp.FindPath(*grid.maze[enemy->currentGridPosition.x][enemy->currentGridPosition.y], 
									     *grid.maze[player->currentGridPosition.x][player->currentGridPosition.y], grid);

				for (int i = 0; i < path.size(); i++)
				{
					path[i]->isActive = true;
				}

				grid.maze[player->currentGridPosition.x][player->currentGridPosition.y]->isPlayerNode = true;
				grid.maze[enemy->currentGridPosition.x][enemy->currentGridPosition.y]->isEnemyNode = true;*/
			}
			break;
		default:break;
		}
	}
}








int main(int argc, char* argv[])
{
	char title[10] = "GRID";
	initSDL(title);
	init();

	while (!game.loopDone) {
		update();
		updateInput();
		draw();
	}

	closeDownSDL();
	return 0;
}