#pragma once
#include <chrono>

#include "../UI/Panel.h"

// https://refactoring.guru/design-patterns/singleton/cpp/example
// We go with Naive Singleton for now
class GameStatistics {
protected:
    GameStatistics();
    static GameStatistics* gameStatistics_;
public:
    // Singleton is not cloneable
    GameStatistics(GameStatistics& other) = delete;
    // Singleton can't be assigned
    void operator=(const GameStatistics&) = delete;

    static GameStatistics* getInstance();

    void updateStatistics(Panel* statisticsPanel);
    void checkGameEnd();

    void setupDefaultStatistics();
    void setupStatistics(int score, int scored, int missed);

    void setWordsGeneralScore(int score);
    int getWordsGeneralScore();
    void increaseWordsGeneralScore(int baseValue);

    void setWordsScored(int scored);
    int getWordsScored();
    void increaseWordsScored(int baseValue);

    void setWordsMissed(int missed);
    int getWordsMissed();
    void increaseWordsMissed(int baseValue);

    void updateAverageTimePerWord();
    void setAverageTimePerWord(std::chrono::duration<double> time);
    std::chrono::duration<double> getAverageTimePerWord();

    void resetTimePassedSinceStart();
    std::chrono::duration<double> getTimePassedSinceStart();
    std::chrono::time_point<std::chrono::system_clock> getTimeAtStart();
    void setTimePassedSinceStart(std::chrono::time_point<std::chrono::system_clock> time);
    void updateTimePassedSinceStart();

    void setPauseTime(std::chrono::high_resolution_clock::time_point time);
    std::chrono::high_resolution_clock::time_point getPauseTime();

    std::string formatTime(std::chrono::duration<double> time);
private:
    int stat_WordsGeneralScore;
    int stat_WordsScored;
    int stat_WordsMissed;

    std::chrono::duration<double> stat_averageTimePerWord{};
    std::chrono::time_point<std::chrono::system_clock> stat_timeAtStart;
    std::chrono::time_point<std::chrono::system_clock> pauseTime;
};