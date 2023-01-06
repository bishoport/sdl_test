#include <SDL.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <SDL_image.h>
#include <iostream>
#include "initclose.h"
#include "renderer.h"

#include "Timer.h"
//#include "TimerPause.h"


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
Grid* grid;
ReferencesManager* ReferencesManager::_instance;

int currentTurn = 0;
Timer* gloabalTimer;
TimerPause* timerPauseCoroutine;

EnemyCharacter* enemyTurn;

bool playerTurn = true;

void calculeNextTurn(); //forward declaration

void init()
{
	grid = new Grid();

	game.loopDone = 0;
	SDL_PumpEvents();  // make sure we have the latest mouse state

	//Player
	PlayerCharacter* player;
	player = new PlayerCharacter( "assets/sprites/player_sprite_sheet.png",grid);
	player->init();
	player->SetGridPosition({grid->startPlayerNode->y,grid->startPlayerNode->x});//+ Grid::SIZE_NODE / 2;
	grid->maze[grid->startPlayerNode->x][grid->startPlayerNode->y]->isPlayerNode = true;
	player->transform.scale.x = Grid::SIZE_NODE;
	player->transform.scale.y = Grid::SIZE_NODE;
	ReferencesManager::getInstance()->setPlayerCharacter(player);
	grid->setDiscoveredByRange(grid->startPlayerNode->y, grid->startPlayerNode->x, 2);


	//Enemies
	for (int i = 0; i < grid->startEnemiesNodes.size(); i++)
	{
		EnemyCharacter* enemy;
		enemy = new EnemyCharacter("assets/sprites/enemies_sprite_sheet.png", grid);
		enemy->init();
		enemy->SetGridPosition({ grid->startEnemiesNodes[i]->y,grid->startEnemiesNodes[i]->x});
		enemy->transform.scale.x = Grid::SIZE_NODE;
		enemy->transform.scale.y = Grid::SIZE_NODE;
		ReferencesManager::getInstance()->enemies.push_back(enemy);
	}

	//TIMERS CONTROLLERS
	gloabalTimer = new Timer();
	timerPauseCoroutine = new TimerPause();

	calculeNextTurn();
	playerTurn = true;
}





void draw()
{
	SDL_SetRenderDrawColor(getRenderer(), 0x3b, 0x3b, 0x3b, 0x00);
	SDL_RenderClear(getRenderer());

	//DRAW STUFFS
	grid->displayMaze();

	ReferencesManager::getInstance()->getPlayerCharacter()->draw();

	grid->maze[ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.x][ReferencesManager::getInstance()->getPlayerCharacter()->currentGridPosition.y]->isPlayerNode = true;

	for (int i = 0; i < ReferencesManager::getInstance()->enemies.size(); i++)
	{
		if (dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[i]))
		{
			EnemyCharacter* enemy = dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[i]);

			if (enemy->isDead == false)
			{
				ReferencesManager::getInstance()->enemies[i]->draw();
				grid->maze[enemy->currentGridPosition.x][enemy->currentGridPosition.y]->isEnemyNode = true;
			}
		}
	}

	SDL_RenderPresent(getRenderer());
	//Don't be a CPU HOG!! :D
	SDL_Delay(1);
}



void update()
{
	SDL_GetMouseState(&game.mouseRect.x, &game.mouseRect.y);
	
	//TIME UPDATE
	timerPauseCoroutine->update(gloabalTimer->GetDeltaTime());

	//PLAYER UPDATE
	ReferencesManager::getInstance()->getPlayerCharacter()->update(gloabalTimer->GetDeltaTime());

	//ENEMIES UPDATE
	for (int i = 0; i < ReferencesManager::getInstance()->enemies.size(); i++)
	{
		if (dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[i]))
		{
			EnemyCharacter* enemy = dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[i]);
			/*if (enemy->isDead == false)
			{*/
				enemy->update(gloabalTimer->GetDeltaTime());
			//}
		}
	}
}



void moveAllEnemies()
{
	if (playerTurn == true)
	{
		cout << "ES EL TURNO DEL PLAYER, NO DEBERIA PASAR POR AQUI" << endl;
		return;
	}

	if (grid->maze[enemyTurn->currentGridPosition.x][enemyTurn->currentGridPosition.y]->isDiscovered)
	{
		enemyTurn->move();
		timerPauseCoroutine->StartPause([]() {
			calculeNextTurn();
		},20);
	}
	else
	{
		timerPauseCoroutine->StartPause([]() {
			calculeNextTurn();
		},1);
	}
	
}

void calculeNextTurn()
{
	for (int i = 0; i < ReferencesManager::getInstance()->enemies.size(); i++)
	{
		if (dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[i]))
		{
			EnemyCharacter* enemy = dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[i]);

			if (enemy->isDead)
			{
				//Eliminamos de la lista el enemigo si está muerto
				ReferencesManager::getInstance()->enemies.erase(std::remove(ReferencesManager::getInstance()
					->enemies.begin(), ReferencesManager::getInstance()->enemies.end(),
					enemy), ReferencesManager::getInstance()->enemies.end());
			}
		}
	}

	if (ReferencesManager::getInstance()->enemies.size() == 0)
	{
		cout << "END GAME" << endl;
	}
	else
	{
		if (dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[currentTurn]))
		{
			enemyTurn = dynamic_cast<EnemyCharacter*>(ReferencesManager::getInstance()->enemies[currentTurn]);
		}


		if (currentTurn == ReferencesManager::getInstance()->enemies.size() -1)
		{
			ReferencesManager::getInstance()->getPlayerCharacter()->secondActionSelected = -1;
			playerTurn = true;
			currentTurn = 0;
		}
		else
		{
			moveAllEnemies(); //<-Recursive call
			currentTurn += 1;
		}

		cout << "CURRENT TURN->" << currentTurn << endl;
	}
}





void firstActionInput(const char* axis)
{

}

//bool firstActionSelected = false;



void MovePlayer(const char* firstActionSelected , int secondActionSelected )
{
	ReferencesManager::getInstance()->getPlayerCharacter()->firstActionSelected = firstActionSelected; 
	ReferencesManager::getInstance()->getPlayerCharacter()->secondActionSelected = secondActionSelected;
	ReferencesManager::getInstance()->getPlayerCharacter()->MoveFromInput();
	timerPauseCoroutine->StartPause([]() {
		moveAllEnemies();
		}, 20);
	playerTurn = false;
	//firstActionSelected = false;
}

//-1 -> None
	// 0 -> Only Move DOWN
	// 1 -> near attack RIGHT
	// 2 -> far attack UP
	// 3 -> with shiled LEFT

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
				if (playerTurn)
				{
					MovePlayer("-Y", 1);

					//if (firstActionSelected == false)
					//{
					//	ReferencesManager::getInstance()->getPlayerCharacter()->firstActionSelected = "-Y"; //IZQUIERDA
					//	firstActionSelected = true;
					//}
					//else 
					//{
					//	ReferencesManager::getInstance()->getPlayerCharacter()->secondActionSelected = 3;
					//	ReferencesManager::getInstance()->getPlayerCharacter()->MoveFromInput();
					//	timerPauseCoroutine->StartPause([]() {
					//		moveAllEnemies();
					//		}, 20);
					//	playerTurn = false;
					//	firstActionSelected = false;
					//}
				}
				break;



			case SDLK_d:
				if (playerTurn)
				{
					MovePlayer("+Y", 1);

					//if (firstActionSelected == false)
					//{
					//	ReferencesManager::getInstance()->getPlayerCharacter()->firstActionSelected = "+Y"; //DERECHA
					//	firstActionSelected = true;
					//}
					//else
					//{
					//	ReferencesManager::getInstance()->getPlayerCharacter()->secondActionSelected = 1;
					//	ReferencesManager::getInstance()->getPlayerCharacter()->MoveFromInput();
					//	timerPauseCoroutine->StartPause([]() {
					//		moveAllEnemies();
					//		}, 20);
					//	playerTurn = false;
					//	firstActionSelected = false;
					//}
				}
				break;

			case SDLK_w:
				if (playerTurn)
				{
					MovePlayer("-X", 1);


					//if (firstActionSelected == false)
					//{
					//	ReferencesManager::getInstance()->getPlayerCharacter()->firstActionSelected = "-X";  //ARRIBA
					//	firstActionSelected = true;
					//}
					//else
					//{
					//	ReferencesManager::getInstance()->getPlayerCharacter()->secondActionSelected = 2;
					//	ReferencesManager::getInstance()->getPlayerCharacter()->MoveFromInput();
					//	timerPauseCoroutine->StartPause([]() {
					//		moveAllEnemies();
					//		}, 20);
					//	playerTurn = false;
					//	firstActionSelected = false;
					//}
				}
				break;

			case SDLK_s:
				if (playerTurn)
				{
					MovePlayer("+X", 1);

					//if (firstActionSelected == false)
					//{
					//	ReferencesManager::getInstance()->getPlayerCharacter()->firstActionSelected = "+X"; //ABAJO
					//	firstActionSelected = true;
					//}
					//else
					//{
					//	ReferencesManager::getInstance()->getPlayerCharacter()->secondActionSelected = 0;
					//	ReferencesManager::getInstance()->getPlayerCharacter()->MoveFromInput();
					//	timerPauseCoroutine->StartPause([]() {
					//		moveAllEnemies();
					//		}, 20);
					//	playerTurn = false;
					//	firstActionSelected = false;
					//}
				}
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
		gloabalTimer->Tick();
		//Update screen
		
	
	}

	closeDownSDL();
	return 0;
}