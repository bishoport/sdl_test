#ifndef ENEMY_CHARACTER_H
#define ENEMY_CHARACTER_H

#include "Character.h"
#include <SDL_image.h>
#include "Pathfinding.h"
#include "ReferencesManager.h"



class EnemyCharacter : public Character
{
public:
	EnemyCharacter();
	~EnemyCharacter();
	EnemyCharacter(const char* _filename, Grid& _grid);
	void Move();
	int lifePoint = 10;
	bool isDead = false;
	

	void DoDamage(int value);
	//bool damageTimer(int num);
private:
	Pathfinding pathfinding;
	vector<Grid::Node*> path;

	
	
};

#endif

