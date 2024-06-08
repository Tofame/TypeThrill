#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>

// https://refactoring.guru/design-patterns/singleton/cpp/example
// We go with Naive Singleton for now
class GameSaveManager {
protected:
    GameSaveManager();
    static GameSaveManager* gameSaveManager_;

public:
    // Singleton is not cloneable
    GameSaveManager(GameSaveManager& other) = delete;
    // Singleton can't be assigned
    void operator=(const GameSaveManager&) = delete;

    static GameSaveManager* getInstance();

    bool loadGameFromFile(int saveSlot);
    bool saveGameToFile(int saveSlot, std::string timePassed);

    void saveGame(int saveSlot);
    void loadGame(int saveSlot);

    void preloadInitialInformation();
    void updateSlotUIElements();

    void setSavedGameInformation(int slotIndex, std::vector<std::string> params);
    std::vector<std::string> getSavedGameInformation(int slotIndex);
    std::string getSlotDescription(int slotIndex);

    // A lambda that is properly initialized in GameInterface::setupUI
    // It will hide the save slots that are visible in Pause Panel during the game
    std::function<void()> lambdaHideSaveSlots = []() -> void {};
private:
    // Contains 'maxSaveSlots' keys after calling GameSaveManager::preloadInitialInformation()
    // Depending on existence of a game save file it will either have a string vetor containing "empty" at index 0
    // or it will have all statistics - 0 = wordsScored, 1 = wordsMissed, 2 = generalScore, 3 = timePassed
    std::map<int, std::vector<std::string>> savedGamesInformation;
};