#pragma once

#include <unordered_map>
#include <vector>
#include <string>

struct Highscores {
    static void loadHighscores();

    static std::vector<std::string>& getHighscore(int index);
    static void setHighscore(int index, std::vector<std::string> highscoreVecString);

private:
    static std::unordered_map<int, std::vector<std::string>> highscores;
};