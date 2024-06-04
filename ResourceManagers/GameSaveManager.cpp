#include "GameSaveManager.h"
#include "../Globals.h"

#include <fstream>
#include <regex>
#include <fmt/format.h>

#include "../Settings.h"
#include "../Game/Game.h"
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

    std::regex patternSettings("([a-zA-Z_]+)[=]{1}(.+)");
    std::smatch matches;

    for (auto line = std::string(); std::getline(file, line); ) {
        if(line.empty() || line[0] == '/')
            continue;

        if(line == ">>settings") {
            while(std::getline(file, line) && line != ">>settingsEnd") {
                if (std::regex_match(line, matches, patternSettings)) {
                    std::string option = matches[1].str();
                    std::string value = matches[2].str();

                    if (Settings::settingsMap.contains(option)) {
                        Settings::settingsMap[option] = value;
                    } else {
                        throw std::runtime_error(fmt::format("Settings.txt option: {} does not exist in settingsMap.\nThe full line is: {}", option, matches[0].str()));
                    }
                } else {
                    throw std::runtime_error("Malformed settings part line: " + line + " of save file: save" + std::to_string(saveSlot));
                }
            }
        }

        if(line == ">>statistics") {
            while(std::getline(file, line) && line != ">>statisticsEnd") {
                //if(line.starts_with())
                // To be continued
            }
        }
    }

    Game::setGameState(Game::STATE_PLAYING, true);
}
