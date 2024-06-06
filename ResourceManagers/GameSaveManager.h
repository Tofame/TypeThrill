#pragma once

// https://refactoring.guru/design-patterns/singleton/cpp/example
// We go with Naive Singleton for now
class GameSaveManager {
protected:
    GameSaveManager() = default;
    static GameSaveManager* gameSaveManager_;

public:
    // Singleton is not cloneable
    GameSaveManager(GameSaveManager& other) = delete;
    // Singleton can't be assigned
    void operator=(const GameSaveManager&) = delete;

    static GameSaveManager* getInstance();

    void loadGameFromFile(int saveSlot);
    void saveGameToFile(int saveSlot);

    void saveGame(int saveSlot);
    void loadGame(int saveSlot);
};