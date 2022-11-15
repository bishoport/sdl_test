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


//void ReferencesManager::setEnemyCharacter(EnemyCharacter* _enemy)
//{
//	enemy = _enemy;
//}
//
//EnemyCharacter* ReferencesManager::getEnemyCharacter()
//{
//	return enemy;
//}

