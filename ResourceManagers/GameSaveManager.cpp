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


GameSaveManager* GameSaveManager::getInstance()
{
    if(gameSaveManager_ == nullptr){
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

                    WordSpawner::getInstance()->spawnWord(std::stof(ratioX), std::stof(ratioY), word);
                } else {
                    throw std::runtime_error("Malformed statistics part, line: " + line + " of save file: save" + std::to_string(saveSlot));
                }
            }
        }
    }

    Game::getInstance()->setGameState(STATE_PLAYING, true);
}

void GameSaveManager::saveGame(int saveSlot) {
    if(saveSlot <= 0 || saveSlot > maxSaveSlots) {
        throw std::runtime_error("Tried to load a save file that exceeds the maximum limit. The slot: " + saveSlot);
    }

    // https://stackoverflow.com/questions/17032970/clear-data-inside-text-file-in-c Author: PureW
    // ofstream with clear file opening
    auto file = std::ofstream(projectPath + "/Resources/GameFiles/save" + std::to_string(saveSlot) + ".txt", std::ofstream::out | std::ofstream::trunc);

    if (!file) {
        throw std::runtime_error(fmt::format("Unable to open a saved game file - {}.txt.", std::to_string(saveSlot)));
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
    file << "timePassed=" << GameStatistics::getInstance()->formatTime(GameStatistics::getInstance()->getTimePassedSinceStart()) << "\n";
    file << ">>statisticsEnd" << "\n";

    file << ">>ingame" << "\n";
    std::string textInput = "";
    auto statisticsPanel = GameInterface::getInstance()->getPanelByType(PANEL_GAMESTATISTICS);
    if(statisticsPanel != nullptr) {
        for(auto uielement : statisticsPanel->UIElements) {
            if(uielement->getType() == TEXTFIELD) {
                auto textfield = static_cast<TextField*>(uielement);
                textInput = textfield->getInputString();
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
}
