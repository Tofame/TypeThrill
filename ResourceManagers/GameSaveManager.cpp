#include "GameSaveManager.h"
#include "../Globals.h"

#include <fstream>
#include <regex>
#include <fmt/format.h>

#include "../Settings.h"
#include "../Game/Game.h"
#include "../Game/GameInterface.h"
#include "../Game/GameStatistics.h"
#include "../Game/WordSpawner.h"
auto maxSaveSlots = 3;

GameSaveManager* GameSaveManager::gameSaveManager_= nullptr;;

GameSaveManager* GameSaveManager::getInstance()
{
    if(gameSaveManager_==nullptr){
        gameSaveManager_ = new GameSaveManager();
    }
    return gameSaveManager_;
}

void GameSaveManager::loadGame(int saveSlot) {
    if(saveSlot <= 0 || saveSlot > maxSaveSlots) {
        throw std::runtime_error("Tried to load a save file that exceeds the maximum limit. The slot: " + saveSlot);
    }

    auto file = std::fstream(projectPath + "/Resources/GameFiles/save" + std::to_string(saveSlot) + ".txt");

    if (!file) {
        throw std::runtime_error("Unable to open a saved game file - .txt.");
    }

    // Pattern for everything besides Words
    std::regex pattern("([a-zA-Z_]+)[=]{1}(.+)");

    // Pattern for words as they are e.g.: 0.3,0.2,slon while options are option=value
    std::regex patternWord("([0-1][.][0-9]{1,2}),([0-1][.][0-9]),(.+)");

    std::smatch matches;

    for (auto line = std::string(); std::getline(file, line); ) {
        if(line.empty() || line[0] == '/')
            continue;

        if(line == ">>settingsANDcriteriums") {
            while(std::getline(file, line) && line != ">>settingsANDcriteriumsEnd") {
                if(line.empty() || line[0] == '/')
                    continue;

                if (std::regex_match(line, matches, pattern)) {
                    std::string option = matches[1].str();
                    std::string value = matches[2].str();

                    if (Settings::settingsMap.contains(option)) {
                        Settings::settingsMap[option] = value;
                    } else {
                        throw std::runtime_error(fmt::format("save{}.txt option: {} does not exist in settingsMap.\nThe full line is: {}", std::to_string(saveSlot), option, matches[0].str()));
                    }
                } else {
                    throw std::runtime_error("Malformed settings part, line: " + line + " of save file: save" + std::to_string(saveSlot));
                }
            }
        }

        if(line == ">>statistics") {
            while(std::getline(file, line) && line != ">>statisticsEnd") {
                if(line.empty() || line[0] == '/')
                    continue;

                if (std::regex_match(line, matches, pattern)) {
                    std::string option = matches[1].str();
                    std::string value = matches[2].str();

                    if(option == "wordsScored") {
                        GameStatistics::setWordsScored(std::stoi(value));
                    } else if(option == "wordsMissed") {
                        GameStatistics::setWordsMissed(std::stoi(value));
                    } else if(option == "generalScore") {
                        GameStatistics::setWordsGeneralScore(std::stoi(value));
                    } else if(option == "timePassed") {
                        // https://stackoverflow.com/questions/17899684/convert-seconds-as-double-to-stdchronoduration
                        // Author: Krellan
                        auto timeValue_as_duration = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(std::chrono::duration<double>(std::stod(value)));
                        auto newTime = std::chrono::high_resolution_clock::now() - timeValue_as_duration;

                        GameStatistics::setTimePassedSinceStart(newTime);
                        GameStatistics::updateAverageTimePerWord();
                    }
                } else {
                    throw std::runtime_error("Malformed statistics part, line: " + line + " of save file: save" + std::to_string(saveSlot));
                }
            }
        }

        if(line.starts_with("gameTextFieldInput")) {
            if (std::regex_match(line, matches, pattern)) {
                std::string option = matches[1].str();
                std::string value = matches[2].str();

                auto statisticsPanel = GameInterface::getPanelByType(PANEL_GAMESTATISTICS);
                if(statisticsPanel != nullptr) {
                    for(auto uielement : statisticsPanel->UIElements) {
                        if(uielement->getType() == TEXTFIELD) {
                            auto textfield = static_cast<TextField*>(uielement);
                            textfield->setInput(value);
                            break;
                        }
                    }
                }
            }
        }

        if(line == ">>words") {
            while(std::getline(file, line)) {
                if(line.empty() || line[0] == '/')
                    continue;

                if (std::regex_match(line, matches, patternWord)) {
                    std::string ratioX = matches[1].str();
                    std::string ratioY = matches[2].str();
                    std::string word = matches[3].str();
                    fmt::println("rat {} ray {} wordd {}", ratioX, ratioY, word);

                    WordSpawner::spawnWord(std::stof(ratioX), std::stof(ratioY), word);
                } else {
                    throw std::runtime_error("Malformed statistics part, line: " + line + " of save file: save" + std::to_string(saveSlot));
                }
            }
        }
    }

    Game::setGameState(Game::STATE_PLAYING, true);
}
