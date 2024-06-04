#pragma once

// https://refactoring.guru/design-patterns/singleton/cpp/example
// We go with Naive Singletton for now as I wouldn't 100% defend in college a thread-safe one.
class GameSaveManager {
protected:
    GameSaveManager() = default;
    static GameSaveManager* gameSaveManager_;

public:
    // Singletton is not cloneable
    GameSaveManager(GameSaveManager& other) = delete;
    // Singletton can't be assigned
    void operator=(const GameSaveManager&) = delete;

    static GameSaveManager *getInstance();

    void loadGame(int saveSlot);
    void saveGame(int saveSlot);
};