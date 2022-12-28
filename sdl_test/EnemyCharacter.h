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
	EnemyCharacter(const char* _IdleImage, const char* _DamageImage, Grid& _grid);
	void Move();
	void update(float deltaTime) override;
	void draw() override;
	int lifePoint = 10;
	bool isDead = false;
	
	void DoDamage(int value);
	//bool damageTimer(int num);

private:
	Pathfinding pathfinding;
	vector<Grid::Node*> path;
};

#endif

