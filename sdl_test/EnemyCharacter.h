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
	EnemyCharacter(const char* _filename, Grid* _grid);
	~EnemyCharacter();


	void move() override;
	void update(float deltaTime) override;
	void draw() override;
	int lifePoint = 30;
	
	bool isDead = false;
	bool isInDamage = false;

	bool isPlayerInAgro = false;

	void DoDamage(int value);

	//bool damageTimer(int num);
	
private:


	Pathfinding pathfinding;
	vector<Grid::Node*> path;

	SDL_Rect gSpriteClips[2];
	Spritesheet  gSpriteSheetTexture;
	bool loadMedia(const char* _filename);


};

#endif

