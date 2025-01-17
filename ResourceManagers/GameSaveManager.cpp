#include "GameSaveManager.h"

#include <codecvt>

#include "../Globals.h"

#include <fstream>
#include <regex>
#include <fmt/format.h>

#include "../Settings.h"
#include "../Game/Game.h"
#include "../Game/GameInterface.h"
#include "../Game/GameStatistics.h"
#include "../Game/WordSpawner.h"
#include "../UI/Word.h"

auto maxSaveSlots = 3;
GameSaveManager* GameSaveManager::gameSaveManager_= nullptr;

// https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
// Author: dk123
// Why do I need it?
// It's because I need to save a Word's text that is sf::Text and sf::String.
// I cant with ofstream use wide string. And if I go by other choices which are
// .toAnsiString - e.g. "Ł" is cut (and other characters) || .toUtf8 is not supported with ofstream
std::string ws2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.to_bytes(wstr);
}


GameSaveManager::GameSaveManager() {
    this->savedGamesInformation = std::map<int, std::vector<std::string>>();
}

GameSaveManager* GameSaveManager::getInstance()
{
    if(gameSaveManager_ == nullptr){
        gameSaveManager_ = new GameSaveManager();
    }
    return gameSaveManager_;
}

// Accepts int parameter that will decide which file we will load (its used in building path string)
// Goes through all lines of file and sets settings, criteriums and other loaded values.
// Returns true if game was successfully loaded.
// Shouldn't be called, it's meant to be called only in loadGame() as it's a part of that
bool GameSaveManager::loadGameFromFile(int saveSlot) {
    if(saveSlot <= 0 || saveSlot > maxSaveSlots) {
        throw std::runtime_error("Tried to load a save file that exceeds the maximum limit. The slot: " + saveSlot);
    }

    auto file = std::fstream(projectPath + "/Resources/GameFiles/save" + std::to_string(saveSlot) + ".txt");

    // If file doesn't exist, the save slot is just "empty"
    if (!file) {
        return false;
        //throw std::runtime_error("Unable to open a saved game file - .txt.");
    }

    // Pattern for everything besides Words
    std::regex pattern("([a-zA-Z_]+)[=]{1}(.+)");

    // Pattern for words as they are e.g.: 0.3,0.2,slon while options are option=value
    std::regex patternWord("([0-1][.][0-9]+),([0-1][.][0-9]+),(.+)");

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

                    if (Settings::getInstance()->settingsMap.contains(option)) {
                        Settings::getInstance()->settingsMap[option] = value;
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
                        GameStatistics::getInstance()->setWordsScored(std::stoi(value));
                    } else if(option == "wordsMissed") {
                        GameStatistics::getInstance()->setWordsMissed(std::stoi(value));
                    } else if(option == "generalScore") {
                        GameStatistics::getInstance()->setWordsGeneralScore(std::stoi(value));
                    } else if(option == "timePassed") {
                        // https://stackoverflow.com/questions/17899684/convert-seconds-as-double-to-stdchronoduration
                        // Author: Krellan
                        auto timeValue_as_duration = std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(std::chrono::duration<double>(std::stod(value)));
                        auto newTime = std::chrono::high_resolution_clock::now() - timeValue_as_duration;

                        GameStatistics::getInstance()->setTimePassedSinceStart(newTime);
                        GameStatistics::getInstance()->updateAverageTimePerWord();
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

                auto statisticsPanel = GameInterface::getInstance()->getPanelByType(PANEL_GAMESTATISTICS);
                if(statisticsPanel != nullptr) {
                    for(auto uielement : statisticsPanel->UIElements) {
                        if(uielement->getType() == TEXTFIELD) {
                            auto textfield = static_cast<TextField*>(uielement);
                            sf::String sfString = sf::String::fromUtf8(value.begin(), value.end());
                            textfield->setInput(sfString);
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

                    WordSpawner::getInstance()->spawnWord(std::stof(ratioX), std::stof(ratioY), word);
                } else {
                    throw std::runtime_error("Malformed statistics part, line: " + line + " of save file: save" + std::to_string(saveSlot));
                }
            }
        }
    }

    // Update DynamicTextLabel in GameStatistics
    auto gameStatisticsPanel = GameInterface::getInstance()->getPanelByType(PANEL_GAMESTATISTICS);
    if(gameStatisticsPanel == nullptr) {
        throw std::runtime_error("GameSaveManager::loadGameFromFile() can't seem to find PANEL_GAMESTATISTICS. Does it exist at that time?");
    }

    for(auto uielement : gameStatisticsPanel->UIElements) {
        if(uielement->getType() == DYNAMICTEXTLABEL) {
            uielement->update();
        }
    }

    return true;
}

// Accepts int parameter that will decide which file we will save to (its used in building path string)
// Uses getters to get all needed values and then writes each one to the file.
// Returns true if game was successfully saved.
// Shouldn't be called, it's meant to be called only in saveGame() as it's a part of that
// Requires timePassed as parameter, as it's already calculated in saveGame()
bool GameSaveManager::saveGameToFile(int saveSlot, std::string timePassed) {
    if(saveSlot <= 0 || saveSlot > maxSaveSlots) {
        throw std::runtime_error("Tried to load a save file that exceeds the maximum limit. The slot: " + saveSlot);
    }

    // https://stackoverflow.com/questions/17032970/clear-data-inside-text-file-in-c Author: PureW
    // ofstream with clear file opening
    auto file = std::ofstream(projectPath + "/Resources/GameFiles/save" + std::to_string(saveSlot) + ".txt", std::ofstream::out | std::ofstream::trunc);

    if (!file) {
        return false;
        //throw std::runtime_error(fmt::format("Unable to open a saved game file - {}.txt.", std::to_string(saveSlot)));
    }

    // Writing to file all the statistics, settings etc.
    file << ">>settingsANDcriteriums" << "\n";
    for(auto entryPair : Settings::getInstance()->settingsMap) {
        file << entryPair.first << "=" << entryPair.second << "\n";
    }
    file << ">>settingsANDcriteriumsEnd" << "\n";

    file << ">>statistics" << "\n";
    file << "wordsScored=" << std::to_string(GameStatistics::getInstance()->getWordsScored()) << "\n";
    file << "wordsMissed=" << std::to_string(GameStatistics::getInstance()->getWordsMissed()) << "\n";
    file << "generalScore=" << std::to_string(GameStatistics::getInstance()->getWordsGeneralScore()) << "\n";
    file << "timePassed=" << timePassed << "\n";
    file << ">>statisticsEnd" << "\n";

    file << ">>ingame" << "\n";
    std::string textInput;
    auto statisticsPanel = GameInterface::getInstance()->getPanelByType(PANEL_GAMESTATISTICS);
    if(statisticsPanel != nullptr) {
        for(auto uielement : statisticsPanel->UIElements) {
            if(uielement->getType() == TEXTFIELD) {
                auto textfield = static_cast<TextField*>(uielement);
                textInput = ws2s(textfield->getInputString().toWideString());
                break;
            }
        }
    }
    file << "gameTextFieldInput=" << textInput << "\n";
    file << ">>ingameEnd" << "\n";

    file << ">>words" << "\n";
    auto wordsPanel = GameInterface::getInstance()->getPanelByType(PANEL_WORDS);
    if(wordsPanel != nullptr) {
        for(auto uielement : wordsPanel->UIElements) {
            if(uielement->getType() == WORD) {
                auto word = static_cast<Word*>(uielement);
                file << std::to_string(word->posRatio.getX()) << "," << std::to_string(word->posRatio.getY()) << "," << ws2s(word->getText().getString().toWideString()) << "\n";
            }
        }
    }

    // Closing
    file.close();

    return true;
}

// Handles saving to the save slot
// Saves game to .txt file, updates dynamic text labels, so they show new values in the "save slot"
void GameSaveManager::saveGame(int saveSlot) {
    // We must first add pause time to the variable that holds time passed since start, because otherwise the time 'passes' during pause
    auto timeSinceStartWithPause = GameStatistics::getInstance()->getTimeSinceStartWithPause();
    auto timePassed = std::chrono::high_resolution_clock::now() - timeSinceStartWithPause;

    auto stringTimePassed = GameStatistics::getInstance()->formatTime(timePassed);

    saveGameToFile(saveSlot, stringTimePassed);

    setSavedGameInformation(
        saveSlot,
        {
            "W. Scored: " + std::to_string(GameStatistics::getInstance()->getWordsScored()),
            "W. Missed: " + std::to_string(GameStatistics::getInstance()->getWordsMissed()),
            "Score: " + std::to_string(GameStatistics::getInstance()->getWordsGeneralScore()),
            "Time Passed: " + stringTimePassed
        }
    );
    this->updateSlotUIElements();

    GameSaveManager::getInstance()->lambdaHideSaveSlots();
}

// method that handles loading game, if loadGameFromFile() is true then it
// prepares the game and sets game state to STATE_PLAYING
void GameSaveManager::loadGame(int saveSlot) {
    if(!loadGameFromFile(saveSlot)) {
        return;
    };

    // loadGameFromFile sets the game's text field input that was saved
    // So, we also want the words to have the higlight
    WordSpawner::getInstance()->manageWords();

    Game::getInstance()->setGameState(STATE_PLAYING, true);
    Game::getInstance()->pause();
}

// This method is meant to setup a collection (savedGamesInformation) that will store basic save information and this info
// will be used in load/save buttons in panels. So they show either "<EMPTY>" or some statistics.
// savedGamesInformation comments contain more information on that matter.
void GameSaveManager::preloadInitialInformation() {
    for(auto slotIndex = 1; slotIndex <= maxSaveSlots; slotIndex++) {
        // Initializes vector with empty strings, without it we will have segmentation fault due to calling e.g. [slotIndex][0]
        this->savedGamesInformation[slotIndex] = {"", "", "", ""};

        auto file = std::fstream(projectPath + "/Resources/GameFiles/save" + std::to_string(slotIndex) + ".txt");
        if (!file) {
            this->savedGamesInformation[slotIndex][0] = "empty";
            continue;
        }

        // Pattern for everything besides Words
        std::regex pattern("([a-zA-Z_]+)[=]{1}(.+)");

        std::smatch matches;

        // Helpful variable to break loop below quicker
        bool statisticsObtained = false;

        for (auto line = std::string(); std::getline(file, line); ) {
            if(line == ">>statistics") {
                while(std::getline(file, line) && line != ">>statisticsEnd") {
                    if(line.empty() || line[0] == '/')
                        continue;

                    if (std::regex_match(line, matches, pattern)) {
                        std::string option = matches[1].str();
                        std::string value = matches[2].str();

                        if(option == "wordsScored") {
                            this->savedGamesInformation[slotIndex][0] = "W. Scored: " + value;
                        } else if(option == "wordsMissed") {
                            this->savedGamesInformation[slotIndex][1] = "W. Missed: " + value;
                        } else if(option == "generalScore") {
                            this->savedGamesInformation[slotIndex][2] = "Score: " + value;
                        } else if(option == "timePassed") {
                            this->savedGamesInformation[slotIndex][3] = "Time Passed: " + value;
                        }
                    } else {
                        throw std::runtime_error("[GameSaveManager::preloadInitialInformation()] Malformed statistics part, line: " + line + " of save file: save" + std::to_string(slotIndex));
                    }
                }
                statisticsObtained = true;
            }

            // We stop looping through lines in this save
            if(statisticsObtained) break;
        }
    }

    this->updateSlotUIElements();
}

// Calls update on all dynamic text labels in PANEL_LOADGAME, PANEL_PAUSE
// (so after saving/loading they show correct info)
void GameSaveManager::updateSlotUIElements() {
    auto loadSlots = std::vector<UIElement*>(3);
    auto saveSlots = std::vector<UIElement*>(3);

    auto loadGamePanel = GameInterface::getInstance()->getPanelByType(PANEL_LOADGAME);
    if(loadGamePanel == nullptr) {
        throw std::runtime_error("GameSaveManager::updateSlotUIElements() couldn't find PANEL_LOADGAME");
    }
    auto pausePanel = GameInterface::getInstance()->getPanelByType(PANEL_PAUSE);
    if(pausePanel == nullptr) {
        throw std::runtime_error("GameSaveManager::updateSlotUIElements() couldn't find PANEL_PAUSE");
    }

    for(auto uielement : loadGamePanel->UIElements) {
        if(uielement->getType() == DYNAMICTEXTLABEL) {
            uielement->update();
        }
    }

    for(auto uielement : pausePanel->UIElements) {
        if(uielement->getType() == DYNAMICTEXTLABEL) {
            uielement->update();
        }
    }
}

// Method simplifying process of updating savedGamesInformation map
// that stores information of save slots (e.g. missed words etc.)
// Example usage: setSavedGameInformation(1, {"wordsMissed: 20", "wordsScored: 10"};
// It is used in saveGame()
void GameSaveManager::setSavedGameInformation(int slotIndex, std::vector<std::string> params) {
    if(slotIndex <= 0 || slotIndex > maxSaveSlots) {
        throw std::runtime_error("GameSaveManager::getSavedGameInformation() index "
                            + std::to_string(slotIndex) + " is out of bounds (" + std::to_string(maxSaveSlots) + ")");
    }

    savedGamesInformation[slotIndex] = params;
}

// Returns vector of strings, each of them being an information that will be used in save slot's description
// (dynamic text label).
std::vector<std::string> GameSaveManager::getSavedGameInformation(int slotIndex) {
    if(slotIndex <= 0 || slotIndex > maxSaveSlots) {
        throw std::runtime_error("GameSaveManager::getSavedGameInformation() index "
                            + std::to_string(slotIndex) + " is out of bounds (" + std::to_string(maxSaveSlots) + ")");
    }

    if(savedGamesInformation.contains(slotIndex)) {
        return savedGamesInformation[slotIndex];
    }

    return {};
}

// Method to be used by Dynamic Text Labels, so they will get ready to display string returned by this method
// If the vector returned by getSavedGameInformation() doesn't contain anything then "EMPTY" string is returned.
std::string GameSaveManager::getSlotDescription(int slotIndex) {
    std::string desc;
    auto infoVec = GameSaveManager::getInstance()->getSavedGameInformation(slotIndex);
    if(infoVec.empty() == false) {
        for(auto info : infoVec) {
            desc += info + "\n";
        }
    } else {
        desc = "EMPTY";
    }

    return desc;
}
