#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H


#include "Character.h"
#include "Spritesheet.h"




class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	~PlayerCharacter();
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

private:
	SDL_Rect gSpriteClips[1];
	Spritesheet  gSpriteSheetTexture;
	bool loadMedia(const char* _filename);
};

#endif

