#ifndef _TURN_MANAGER_H
#define _TURN_MANAGER_H

#include "TimerPause.h"
#include "EnemyCharacter.h"

class TurnManager : public TimerPause
{


public :

	EnemyCharacter* nextEnemyTurn;

	TurnManager()
	{
		nextEnemyTurn = nullptr;
	}


	void EndPause() override
	{
		nextEnemyTurn->Move();
	}

};

#endif

