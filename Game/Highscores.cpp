#include "Highscores.h"

#include "../Globals.h"

#include <fstream>
#include <filesystem>
#include <regex>

auto MAXHIGHSCORES_AMOUNT = 10;
std::unordered_map<int, std::vector<std::string>> Highscores::highscores = std::unordered_map<int, std::vector<std::string>>();

void Highscores::loadHighscores() {
    std::string path = projectPath + "/Resources/GameFiles/highscores.txt";

    // Give here link to stackoverflow
    if(std::filesystem::exists(path) == false) {
        std::ofstream newFile(path);

        for(auto i = 1; i <= (MAXHIGHSCORES_AMOUNT - 1); i++) {
            newFile << "empty\n";
            // Each highscore line will look like this:
            // score,timepassed,wordFrequency,wordSpeed,chosenLanguage
            Highscores::setHighscore(i, {"empty", "empty", "empty", "empty", "empty"});
        }
        newFile << "empty";
        Highscores::setHighscore(MAXHIGHSCORES_AMOUNT, {"empty", "empty", "empty", "empty", "empty"});

        newFile.close();
        return;
    }

    auto file = std::fstream(path);

    if (!file) {
        throw std::runtime_error("Unable to open highscores.txt. The path used is: " + path);
    }

    // score,timepassed,wordFrequency,wordSpeed,chosenLanguage
    std::regex pattern("([0-9]+)[,]{1}([]+)");
    std::smatch matches;

    for (auto line = std::string(); std::getline(file, line); ) {
        if(line.empty() || line[0] == '/')
            continue;

        if (std::regex_match(line, matches, pattern)) {
            std::string option = matches[1].str();
            std::string value = matches[2].str();

//            if (Highscores::highscores.contains(option)) {
//                settingsMap[option] = value;
//            } else {
//                throw std::runtime_error(fmt::format("Settings.txt option: {} does not exist in settingsMap.\nThe full line is: {}", option, matches[0].str()));
//            }
        } else {
            //throw std::runtime_error("Malformed settings.txt line: " + line);
        }
    }
}

std::vector<std::string>& Highscores::getHighscore(int index) {
    if(index < 1 || index > 10) {
        throw std::runtime_error("Highscores::getHighscore() method was called with wrong index: " + index);
    }

    return Highscores::highscores[index];
}

void Highscores::setHighscore(int index, std::vector<std::string> highscoreVecString) {
    if(index < 1 || index > 10) {
        throw std::runtime_error("Highscores::setHighscore() method was called with wrong index: " + index);
    }

    Highscores::highscores[index] = highscoreVecString;
}