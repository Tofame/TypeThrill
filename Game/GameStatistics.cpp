#include "GameStatistics.h"

#include "Game.h"
#include "GameInterface.h"
#include "../Settings.h"
#include "../UI/DynamicTextLabel.h"
#include "fmt/format.h"

GameStatistics* GameStatistics::gameStatistics_= nullptr;

GameStatistics::GameStatistics() {
    this->stat_WordsGeneralScore = 0;
    this->stat_WordsScored = 0;
    this->stat_WordsMissed = 0;

    this->stat_averageTimePerWord = std::chrono::duration<double>(0);
    this->stat_timeAtStart = std::chrono::high_resolution_clock::now();
    this->pauseTime = std::chrono::high_resolution_clock::now();
}

GameStatistics* GameStatistics::getInstance() {
    if(gameStatistics_== nullptr){
        gameStatistics_ = new GameStatistics();
    }
    return gameStatistics_;
}

void GameStatistics::updateStatistics(Panel* statisticsPanel) {
    if(statisticsPanel == nullptr) {
        throw std::runtime_error("GameStatistics::updateStatistics() can't seem to find PANEL_GAMESTATISTICS. Does it exist?");
    }

    this->updateAverageTimePerWord();

    for(auto uielement : statisticsPanel->UIElements) {
        uielement->update();
    }

    this->checkGameEnd();
}

// Checks if conditions for game over are met.
// If they are then it sets game state to STATE_GAMEOVER.
void GameStatistics::checkGameEnd() {
    if(Settings::getInstance()->getEndGameCriteriumBool("endGame_never_bool") == true) {
        return;
    }

    auto gameEnd = false;

    if(Settings::getInstance()->getEndGameCriteriumBool("endGame_score_bool")) {
        if(this->getWordsGeneralScore() >= Settings::getInstance()->getEndGameCriterium_score()) {
            gameEnd = true;
        }
    }

    if(!gameEnd && Settings::getInstance()->getEndGameCriteriumBool("endGame_missedWords_bool")) {
        if(this->getWordsMissed() >= Settings::getInstance()->getEndGameCriterium_missedWords()) {
            gameEnd = true;
        }
    }

    if(!gameEnd && Settings::getInstance()->getEndGameCriteriumBool("endGame_time_bool")) {
        if(this->getTimePassedSinceStart() >= Settings::getInstance()->getEndGameCriterium_time()) {
            gameEnd = true;
        }
    }

    if(gameEnd == true) {
        Game::getInstance()->setGameState(STATE_GAMEOVER, false);
        Game::getInstance()->onGameOver();
    }
}

void GameStatistics::setupDefaultStatistics() {
    this->setWordsGeneralScore(0);
    this->setWordsScored(0);
    this->setWordsMissed(0);

    this->resetTimePassedSinceStart();
    this->updateAverageTimePerWord();

    this->updateStatistics(GameInterface::getInstance()->getPanelByType(PANEL_GAMESTATISTICS));
}

void GameStatistics::setupStatistics(int score, int scored, int missed) {
    this->setWordsGeneralScore(score);
    this->setWordsScored(scored);
    this->setWordsMissed(missed);
    // time since start
    this->updateAverageTimePerWord();
    this->updateStatistics(GameInterface::getInstance()->getPanelByType(PANEL_GAMESTATISTICS));
}

void GameStatistics::setWordsGeneralScore(int score) {
    stat_WordsGeneralScore = score;
}

int GameStatistics::getWordsGeneralScore() {
    return stat_WordsGeneralScore;
}

void GameStatistics::increaseWordsGeneralScore(int baseValue) {
    auto value = baseValue; // Placeholder for future enhancements
    this->setWordsGeneralScore(this->getWordsGeneralScore() + value);
}

void GameStatistics::setWordsScored(int scored) {
    stat_WordsScored = scored;
}

int GameStatistics::getWordsScored() {
    return stat_WordsScored;
}

void GameStatistics::increaseWordsScored(int baseValue) {
    auto value = baseValue; // Placeholder for future enhancements
    this->setWordsScored(this->getWordsScored() + value);
}

void GameStatistics::setWordsMissed(int missed) {
    stat_WordsMissed = missed;
}

int GameStatistics::getWordsMissed() {
    return stat_WordsMissed;
}

void GameStatistics::increaseWordsMissed(int baseValue) {
    auto value = baseValue; // Placeholder for future enhancements
    this->setWordsMissed(this->getWordsMissed() + value);
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

void GameStatistics::setAverageTimePerWord(std::chrono::duration<double> time) {
    this->stat_averageTimePerWord = time;
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

std::chrono::time_point<std::chrono::system_clock> GameStatistics::getTimeAtStart() {
    return this->stat_timeAtStart;
}

void GameStatistics::setTimePassedSinceStart(std::chrono::time_point<std::chrono::system_clock> time) {
    stat_timeAtStart = time;
}

void GameStatistics::updateTimePassedSinceStart() {
    this->updateStatistics(GameInterface::getInstance()->getPanelByType(PANEL_GAMESTATISTICS));
}

void GameStatistics::setPauseTime(std::chrono::high_resolution_clock::time_point time) {
    pauseTime = time;
}

std::chrono::high_resolution_clock::time_point GameStatistics::getPauseTime() {
    return pauseTime;
}

// Makes calculations so time at start is coupled with pause time, so time doesn't 'pass' during pause
std::chrono::high_resolution_clock::time_point GameStatistics::getTimeSinceStartWithPause() {
    return getTimeAtStart() + (std::chrono::high_resolution_clock::now() - getPauseTime());
}

std::string GameStatistics::formatTime(std::chrono::duration<double> time) {
    double seconds = time.count();

    return fmt::format("{:.2f}", seconds);
}
