#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include <SDL_image.h>
#include "Character.h"




class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	PlayerCharacter(const char* _filename, Grid& _grid);
	void Move(const char* axis);

	int controlCooldown = 0;

	void update(float deltaTime) override;
	void draw() override;

	Uint64 tick;

	//my animation linear interpol function
	double GetPos(double started, double begin, double end, double duration)
	{
		return (end - begin) * (double)(SDL_GetTicks() - started) / duration + begin;

	}
};

#endif

