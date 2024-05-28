#include "GameStatistics.h"

#include "Game.h"
#include "GameInterface.h"
#include "../Settings.h"
#include "../UI/DynamicTextLabel.h"
#include "fmt/format.h"

int GameStatistics::stat_WordsGeneralScore = 0;
int GameStatistics::stat_WordsScored = 0;
int GameStatistics::stat_WordsMissed = 0;

void GameStatistics::updateStatistics(Panel* statisticsPanel) {
    if(statisticsPanel == nullptr) {
        throw std::runtime_error("GameStatistics::updateStatistics() can't seem to find PANEL_GAMESTATISTICS. Does it exist?");
    }

    updateAverageTimePerWord();

    for(auto uielement : statisticsPanel->UIElements) {
        uielement->update();
    }

    GameStatistics::checkGameEnd();
}

void GameStatistics::checkGameEnd() {
    if(Settings::getEndGameCriteriumBool("endGame_never_bool") == true) {
        return;
    }

    auto gameEnd = false;

    if(Settings::getEndGameCriteriumBool("endGame_score_bool")) {
        if(getWordsGeneralScore() >= Settings::getEndGameCriterium_score()) {
            gameEnd = true;
        }
    }

    if(!gameEnd && Settings::getEndGameCriteriumBool("endGame_missedWords_bool")) {
        if(getWordsMissed() >= Settings::getEndGameCriterium_missedWords()) {
            gameEnd = true;
        }
    }

    if(!gameEnd && Settings::getEndGameCriteriumBool("endGame_time_bool")) {
        if(getTimePassedSinceStart() >= Settings::getEndGameCriterium_time()) {
            gameEnd = true;
        }
    }

    if(gameEnd == true) {
        Game::setGameState(Game::STATE_GAMEOVER, false);
    }
}

void GameStatistics::setupDefaultStatistics() {
    stat_WordsGeneralScore = 0;
    stat_WordsScored = 0;
    stat_WordsMissed = 0;
    resetTimePassedSinceStart();
    updateAverageTimePerWord();
    GameStatistics::updateStatistics(GameInterface::getPanelByType(PANEL_GAMESTATISTICS));
}

void GameStatistics::setupStatistics(int score, int scored, int missed) {
    stat_WordsGeneralScore = score;
    stat_WordsScored = scored;
    stat_WordsMissed = missed;
    // time since start
    updateAverageTimePerWord();
    GameStatistics::updateStatistics(GameInterface::getPanelByType(PANEL_GAMESTATISTICS));
}

void GameStatistics::setWordsGeneralScore(int score) {
    stat_WordsGeneralScore = score;
}

int GameStatistics::getWordsGeneralScore() {
    return stat_WordsGeneralScore;
}

void GameStatistics::increaseWordsGeneralScore(int baseValue) {
    auto value = baseValue; // Placeholder for future enhancements
    stat_WordsGeneralScore += value;
}

void GameStatistics::setWordsScored(int scored) {
    stat_WordsScored = scored;
}

int GameStatistics::getWordsScored() {
    return stat_WordsScored;
}

void GameStatistics::increaseWordsScored(int baseValue) {
    auto value = baseValue;
    stat_WordsScored += value;
}

void GameStatistics::setWordsMissed(int missed) {
    stat_WordsMissed = missed;
}

int GameStatistics::getWordsMissed() {
    return stat_WordsMissed;
}

void GameStatistics::increaseWordsMissed(int baseValue) {
    auto value = baseValue;
    stat_WordsMissed += value;
}

void GameStatistics::updateAverageTimePerWord() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = currentTime - stat_timeAtStart;

    if (getWordsScored() > 0) {
        stat_averageTimePerWord = std::chrono::duration<double>((std::chrono::duration<double>(getWordsScored()) / std::chrono::duration<double>(elapsedTime)));
    } else {
        stat_averageTimePerWord = std::chrono::duration<double>(0);
    }
}

std::chrono::duration<double> GameStatistics::getAverageTimePerWord() {
    return stat_averageTimePerWord;
}

void GameStatistics::resetTimePassedSinceStart() {
    stat_timeAtStart = std::chrono::high_resolution_clock ::now();
}

std::chrono::duration<double> GameStatistics::getTimePassedSinceStart() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    return currentTime - stat_timeAtStart;
}

void GameStatistics::updateTimePassedSinceStart() {
    GameStatistics::updateStatistics(GameInterface::getPanelByType(PANEL_GAMESTATISTICS));
}

std::string GameStatistics::formatTime(std::chrono::duration<double> time) {
    double seconds = time.count();

    return fmt::format("{:.2f}", seconds);
}