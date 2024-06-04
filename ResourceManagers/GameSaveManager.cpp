#include "GameSaveManager.h"

GameSaveManager* GameSaveManager::gameSaveManager_= nullptr;;

GameSaveManager* GameSaveManager::getInstance()
{
    if(gameSaveManager_==nullptr){
        gameSaveManager_ = new GameSaveManager();
    }
    return gameSaveManager_;
}
