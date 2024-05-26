#pragma once
#include <chrono>

#include "../UI/Panel.h"

class GameStatistics {
public:
    static void updateStatistics(Panel* statisticsPanel);

    static void setupDefaultStatistics();
    static void setupStatistics(int score, int scored, int missed);

    static void setWordsGeneralScore(int score);
    static int getWordsGeneralScore();
    static void increaseWordsGeneralScore(int baseValue);

    static void setWordsScored(int scored);
    static int getWordsScored();
    static void increaseWordsScored(int baseValue);

    static void setWordsMissed(int missed);
    static int getWordsMissed();
    static void increaseWordsMissed(int baseValue);

    static void updateAverageTimePerWord();
    static std::chrono::duration<double> getAverageTimePerWord();

    static void resetTimePassedSinceStart();
    static std::chrono::duration<double> getTimePassedSinceStart();
    static void updateTimePassedSinceStart();

    static std::string formatTime(std::chrono::duration<double> time);
private:
    static int stat_WordsGeneralScore;
    static int stat_WordsScored;
    static int stat_WordsMissed;

    // Undefined reference if initialized in GameStatistics.cpp
    inline static std::chrono::duration<double> stat_averageTimePerWord = std::chrono::duration<double>(0);;
    inline static auto stat_timeAtStart = std::chrono::high_resolution_clock::now();
};