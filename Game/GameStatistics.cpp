#include "GameStatistics.h"

int GameStatistics::stat_WordsGeneralScore = 0;
int GameStatistics::stat_WordsScored = 0;
int GameStatistics::stat_WordsMissed = 0;

void GameStatistics::setupDefaultStatistics() {
    stat_WordsGeneralScore = 0;
    stat_WordsScored = 0;
    stat_WordsMissed = 0;
}

void GameStatistics::setupStatistics(int score, int scored, int missed) {
    stat_WordsGeneralScore = score;
    stat_WordsScored = scored;
    stat_WordsMissed = missed;
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