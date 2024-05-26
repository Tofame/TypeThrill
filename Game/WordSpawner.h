#pragma once
#include <chrono>

class WordSpawner {
public:
    static void spawnWord();
    static bool canSpawnWord();

    static void moveWords();

    static void manageWords();
private:
    inline static std::chrono::milliseconds lastSpawnTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());;
};