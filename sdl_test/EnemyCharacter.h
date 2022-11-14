#ifndef ENEMY_CHARACTER_H
#define ENEMY_CHARACTER_H



#include "Character.h"
#include <SDL_image.h>
#include "Pathfinding.h"
#include "PlayerCharacter.h"

class EnemyCharacter : public Character
{
public:
	EnemyCharacter();
	EnemyCharacter(const char* _filename, Grid& _grid);
	void Move();

private:
	Pathfinding pathfinding;
	vector<Grid::Node*> path;
};

#endif

