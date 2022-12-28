#ifndef ENEMY_CHARACTER_H
#define ENEMY_CHARACTER_H

#include "Character.h"
#include "Spritesheet.h"
#include "Pathfinding.h"
#include "ReferencesManager.h"



class EnemyCharacter : public Character
{
public:
	EnemyCharacter();
	~EnemyCharacter();
	EnemyCharacter(const char* _filename, Grid& _grid);
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

	SDL_Rect gSpriteClips[1];
	Spritesheet  gSpriteSheetTexture;
	bool loadMedia(const char* _filename);
};

#endif

