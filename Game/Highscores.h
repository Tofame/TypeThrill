#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "../UI/Panel.h"

struct Highscores {
    static void loadHighscores();
    static void saveHighscores();

    static std::vector<std::string>& getHighscore(int index);
    static void setHighscore(int index, std::vector<std::string> highscoreVecString);
private:
    static std::vector<std::vector<std::string>> highscores;
};