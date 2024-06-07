#pragma once
#include <chrono>
#include <string>
#include <unordered_map>

#include "UI/TextField.h"

// https://refactoring.guru/design-patterns/singleton/cpp/example
// We go with Naive Singleton for now
struct Settings {
protected:
    Settings();
    static Settings* settings_;
public:
    // Singleton is not cloneable
    Settings(Settings& other) = delete;
    // Singleton can't be assigned
    void operator=(const Settings&) = delete;

    static Settings* getInstance();

    std::unordered_map<std::string, std::string> defaultSettingsMap;
    std::unordered_map<std::string, std::string> settingsMap;

    void preLoadSettings(bool resetDefaultSettings);
    void loadSettings();
    void saveSettings();

    void loadSetingsOnNewGame();

    void restoreDefaultSettings();
    void saveSettingsPanel();

    void setWordsFontName(std::string const& value);
    std::string getWordsFontName(bool defaultValue);

    void setWordsFrequency(std::string const& value);
    double getWordsFrequency(bool defaultValue);

    void setWordsSpeed(std::string const& value);
    double getWordsSpeed(bool defaultValue);

    void setWordsSize(std::string const& value);
    double getWordsSize(bool defaultValue);

    void setWordsHighlight(std::string const& value);
    void setWordsHighlight(bool const& value);
    bool isWordsHighlightEnabled(bool defaultValue);

    void setUIScale(std::string const& value);
    float getUIScale(bool defaultValue);

    void setWordLocale(std::string value);
    std::string getWordLocale(bool defaultValue);

    std::string buildEndGameSettings();

// Game End Criteriums
    void setEndGameCriteriumBool(std::string const& criterium, bool value);
    bool getEndGameCriteriumBool(std::string const& criterium);

    void setEndGameCriterium_missedWords(int value);
    void setEndGameCriterium_missedWords(std::string value);
    int getEndGameCriterium_missedWords();

    void setEndGameCriterium_time(double value);
    void setEndGameCriterium_time(std::string value);
    std::chrono::duration<double> getEndGameCriterium_time();

    void setEndGameCriterium_score(int value);
    void setEndGameCriterium_score(std::string value);
    int getEndGameCriterium_score();
};
