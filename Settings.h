#pragma once
#include <chrono>
#include <string>
#include <unordered_map>

#include "UI/TextField.h"

struct Settings {
    inline static auto defaultSettingsMap = std::unordered_map<std::string, std::string>();
    inline static auto settingsMap = std::unordered_map<std::string, std::string>();

    void static preLoadSettings(bool resetDefaultSettings);
    void static loadSettings();
    void static saveSettings();

    void static restoreDefaultSettings();
    void static saveSettingsPanel();

    void static setWordsFontName(std::string const& value);
    std::string static getWordsFontName(bool defaultValue);

    void static setWordsFrequency(std::string const& value);
    double static getWordsFrequency(bool defaultValue);

    void static setWordsSpeed(std::string const& value);
    double static getWordsSpeed(bool defaultValue);

    void static setWordsSize(std::string const& value);
    double static getWordsSize(bool defaultValue);

    void static setWordsHighlight(std::string const& value);
    void static setWordsHighlight(bool const& value);
    bool static isWordsHighlightEnabled(bool defaultValue);

    void static setUIScale(std::string const& value);
    float static getUIScale(bool defaultValue);

    void static setWordLocale(std::string& value);
    std::string static getWordLocale(bool defaultValue);

    std::string static buildEndGameSettings();

// Game End Criteriums
    void static setEndGameCriteriumBool(std::string const& criterium, bool value);
    bool static getEndGameCriteriumBool(std::string const& criterium);

    void static setEndGameCriterium_missedWords(int value);
    void static setEndGameCriterium_missedWords(std::string value);
    int static getEndGameCriterium_missedWords();

    void static setEndGameCriterium_time(double value);
    void static setEndGameCriterium_time(std::string value);
    std::chrono::duration<double> static getEndGameCriterium_time();

    void static setEndGameCriterium_score(int value);
    void static setEndGameCriterium_score(std::string value);
    int static getEndGameCriterium_score();
};
