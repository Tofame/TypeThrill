#pragma once
#include <chrono>

#include "../UI/Panel.h"

class GameStatistics {
public:
    static void updateStatistics(Panel* statisticsPanel);
    static void checkGameEnd();

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
    static std::chrono::time_point<std::chrono::system_clock> getTimeAtStart();
    static void setTimePassedSinceStart(std::chrono::time_point<std::chrono::system_clock> time);
    static void updateTimePassedSinceStart();

    static void setPauseTime(std::chrono::high_resolution_clock::time_point time);
    static std::chrono::high_resolution_clock::time_point getPauseTime();

    static std::string formatTime(std::chrono::duration<double> time);
private:
    static int stat_WordsGeneralScore;
    static int stat_WordsScored;
    static int stat_WordsMissed;

    // Undefined reference if initialized in GameStatistics.cpp
    inline static std::chrono::duration<double> stat_averageTimePerWord = std::chrono::duration<double>(0);;
    inline static auto stat_timeAtStart = std::chrono::high_resolution_clock::now();
    inline static auto pauseTime = std::chrono::high_resolution_clock::now();
};