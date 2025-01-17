#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "../UI/Panel.h"

// https://refactoring.guru/design-patterns/singleton/cpp/example
// We go with Naive Singleton for now
struct Highscores {
protected:
    Highscores() = default;
    static Highscores* highscores_;
public:
    // Singleton is not cloneable
    Highscores(Highscores& other) = delete;
    // Singleton can't be assigned
    void operator=(const Highscores&) = delete;

    static Highscores *getInstance();

    void loadHighscores();
    void saveHighscores();

    int updateHighScores();

    std::vector<std::string>& getHighscore(int index);
    void setHighscore(int index, std::vector<std::string> highscoreVecString);

    std::vector<std::vector<std::string>> highscores;
};