#pragma once

class GameStatistics {
public:
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

private:
    static int stat_WordsGeneralScore;
    static int stat_WordsScored;
    static int stat_WordsMissed;
};