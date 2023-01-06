#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H


#include "Character.h"
#include "Spritesheet.h"




class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	~PlayerCharacter();
	PlayerCharacter(const char* _filename, Grid* _grid);
	void MoveFromInput();

	int lifePoint = 100;
	bool isDead = false;

	//-1 -> None
	// 0 -> Only Move DOWN
	// 1 -> near attack RIGHT
	// 2 -> far attack UP
	// 3 -> with shiled LEFT

	const char* firstActionSelected = "";
	int secondActionSelected = -1;

	void move() override;
	void update(float deltaTime) override;
	void draw() override;

	Uint64 tick;

	//my animation linear interpol function
	double GetPos(double started, double begin, double end, double duration)
	{
		return (end - begin) * (double)(SDL_GetTicks() - started) / duration + begin;
	}

	void DoDamage(int value);

private:
	SDL_Rect gSpriteClips[10];
	Spritesheet  gSpriteSheetTexture;
	bool loadMedia(const char* _filename);
};

#endif

