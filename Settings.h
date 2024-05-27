#pragma once
#include <chrono>
#include <string>
#include <unordered_map>

#include "UI/TextField.h"

class Settings {
private:
    std::string static words_fontName;
    double static words_frequency;
    double static words_speed;
    double static words_size;
    bool static words_highlight;

    float static ui_scale;
public:
    inline static auto defaultSettingsMap = std::unordered_map<std::string, std::string>();
    inline static auto settingsMap = std::unordered_map<std::string, std::string>();

    void static preLoadSettings(bool resetDefaultSettings);
    void static loadSettings();
    void static saveSettings();

    void static restoreDefaultSettings();
    void static applySettingsPanel();

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

    std::string static buildEndGameSettings();

// Game End Criterias (no setters and getters - too much code)
    inline bool static endGame_missedWords_bool = true;
    inline bool static endGame_time_bool = false;
    inline bool static endGame_score_bool = false;

    inline bool static endGame_never_bool = true;

    inline int static endGame_missedWords_value = 5;
    inline auto static endGame_time_value = std::chrono::duration<double>(10);
    inline int static endGame_score_value = 20;
};
