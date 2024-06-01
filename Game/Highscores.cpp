#include "Highscores.h"

#include "../Globals.h"
#include "../UI/UIElementFactory.h"
#include "GameStatistics.h"

#include <fstream>
#include <filesystem>
#include <regex>
#include <fmt/core.h>

auto MAXHIGHSCORES_AMOUNT = 5;
std::vector<std::vector<std::string>> Highscores::highscores(MAXHIGHSCORES_AMOUNT);

void Highscores::loadHighscores() {
    std::string path = projectPath + "/Resources/GameFiles/highscores.txt";

    https://stackoverflow.com/questions/478075/creating-files-in-c    James Thompson's comment
    // on how to create files in C++ (create an instance of the ofstream class)
    if(std::filesystem::exists(path) == false) {
        std::ofstream newFile(path);

        for(auto i = 0; i < MAXHIGHSCORES_AMOUNT - 1; i++) {
            // Each highscore line will look like this:
            // score,timepassed,wordFrequency,wordSpeed,chosenLanguage
            newFile << "empty,empty,empty,empty,empty\n";
            Highscores::setHighscore(i, {"empty", "empty", "empty", "empty", "empty"});
        }
        newFile << "empty,empty,empty,empty,empty";
        Highscores::setHighscore(MAXHIGHSCORES_AMOUNT - 1, {"empty", "empty", "empty", "empty", "empty"});

        newFile.close();
        return;
    }

    auto file = std::fstream(path);

    if (!file) {
        throw std::runtime_error("Unable to open highscores.txt. The path used is: " + path);
    }

    // score,timepassed,wordFrequency,wordSpeed,chosenLanguage
    auto index = 0;
    for (auto line = std::string(); std::getline(file, line); ) {
        std::vector<std::string> singleHighscoreVector;
        std::stringstream ss(line);
        std::string highscorePart;

        while (std::getline(ss, highscorePart, ',')) {
            singleHighscoreVector.push_back(highscorePart);
        }

        Highscores::highscores[index] = singleHighscoreVector;
        index++;
        if(index == MAXHIGHSCORES_AMOUNT) break;
    }

    file.close();
}

// We dont have to worry that saveHighscores() creates new empty line (\n) at the end of file,
// because in loadHighscores() we have "if(index == MAXHIGHSCORES_AMOUNT) break;"
void Highscores::saveHighscores() {
    std::string path = projectPath + "/Resources/GameFiles/highscores.txt";
    std::fstream file(path);

    if (!file) {
        throw std::runtime_error("Unable to open highscores.txt. The path used is: " + path);
    }

    for (auto& highscore : Highscores::highscores) {
        for (auto i = 0; i < highscore.size(); ++i) {
            file << highscore[i];
            if (i < highscore.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
}

std::vector<std::string>& Highscores::getHighscore(int index) {
    if(index < 0 || index >= MAXHIGHSCORES_AMOUNT) {
        throw std::runtime_error("Highscores::getHighscore() method was called with wrong index: " + index);
    }

    return Highscores::highscores[index];
}

void Highscores::setHighscore(int index, std::vector<std::string> highscoreVecString) {
    if(index < 0 || index >= MAXHIGHSCORES_AMOUNT) {
        throw std::runtime_error("Highscores::setHighscore() method was called with wrong index: " + index);
    }

    Highscores::highscores[index] = highscoreVecString;
}

void Highscores::updateHighScores() {
    auto score = GameStatistics::getWordsGeneralScore();

    int highscoreBeaten = -1; // No highscore was beaten
    // We do it differently depending on if it was "empty"... or not.
    // In case it WAS NOT empty we delete the 10th highscore and use vector's insert.
    bool overridesEmptyHighscore = false;

    for(auto i = 0; i < Highscores::highscores.size(); i++) {
        auto highscoreInformation = Highscores::highscores[i];
        if(highscoreInformation[0] == "empty") {
            highscoreBeaten = i;
            overridesEmptyHighscore = true;
            break;
        }

        auto highscoreScore = std::stoi(highscoreInformation[0]);
        if(score > highscoreScore) {
            highscoreBeaten = i;
            break;
        }
    }

    if (highscoreBeaten != -1) {
        auto newHighscore = std::vector<std::string>{
                std::to_string(score),
                GameStatistics::formatTime(GameStatistics::getTimePassedSinceStart()),
                std::to_string(Settings::getWordsFrequency(false)),
                std::to_string(Settings::getWordsSpeed(false)),
                Settings::getWordLocale(),
        };

        if (!overridesEmptyHighscore) {
            Highscores::highscores.pop_back();
            Highscores::highscores.insert(Highscores::highscores.begin() + highscoreBeaten, newHighscore);
        } else {
            Highscores::highscores[highscoreBeaten] = newHighscore;
        }

        // Update dynamic text labels
        auto panelHighscores = GameInterface::getPanelByType(PANEL_HIGHSCORES);
        if(panelHighscores != nullptr) {
            panelHighscores->update();
        }
        // Update highscores text file
        Highscores::saveHighscores();
    }
}