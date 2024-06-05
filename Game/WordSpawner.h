#pragma once
#include <chrono>

// https://refactoring.guru/design-patterns/singleton/cpp/example
// We go with Naive Singleton for now
class WordSpawner {
protected:
    WordSpawner() = default;
    static WordSpawner* wordSpawner_;

public:
    // Singleton is not cloneable
    WordSpawner(WordSpawner& other) = delete;
    // Singleton can't be assigned
    void operator=(const WordSpawner&) = delete;

    static WordSpawner *getInstance();

    void spawnWord();
    void spawnWord(float ratioX, float ratioY, std::string text);
    bool canSpawnWord();

    void moveWords();

    void manageWords();
    void clearWords(bool removeWords, bool removeHighlight, bool clearGameTextField);
private:
    std::chrono::milliseconds lastSpawnTime;
};