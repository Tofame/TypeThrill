#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "../UI/Panel.h"

// https://refactoring.guru/design-patterns/singleton/cpp/example
// We go with Naive Singletton for now as I wouldn't 100% defend in college a thread-safe one.
struct Highscores {
protected:
    Highscores() = default;
    static Highscores* highscores_;
public:
    // Singletton is not cloneable
    Highscores(Highscores& other) = delete;
    // Singletton can't be assigned
    void operator=(const Highscores&) = delete;

    static Highscores *getInstance();

    void loadHighscores();
    void saveHighscores();

    int updateHighScores();

    std::vector<std::string>& getHighscore(int index);
    void setHighscore(int index, std::vector<std::string> highscoreVecString);

    std::vector<std::vector<std::string>> highscores;
};