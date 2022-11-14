#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Character.h"

class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	PlayerCharacter(const char* _filename, Grid& _grid);
	void Move(const char* axis);


};

#endif

