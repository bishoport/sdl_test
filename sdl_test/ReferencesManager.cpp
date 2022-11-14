#include "ReferencesManager.h"


ReferencesManager::ReferencesManager()
{

}

void ReferencesManager::setPlayerCharacter(PlayerCharacter* _player)
{
	player = _player;
}

PlayerCharacter* ReferencesManager::getPlayerCharacter()
{
	return player;
}
