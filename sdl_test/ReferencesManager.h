#ifndef REFERENCES_MANAGER_H
#define REFERENCES_MANAGER_H


#include "PlayerCharacter.h"

class ReferencesManager
{
	static ReferencesManager* _instance;
	PlayerCharacter* player;

	ReferencesManager();

public:

	vector<Character*> enemies;
	
	static ReferencesManager* getInstance()
	{
		if (_instance == NULL)
			_instance = new ReferencesManager();
		return _instance;
	}

	void setPlayerCharacter(PlayerCharacter* _player);
	PlayerCharacter* getPlayerCharacter();
};
#endif

