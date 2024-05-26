#pragma once
#include <chrono>
#include <string>

#include "UI/TextField.h"

class Settings {
private:
    std::string static words_fontName;
    double static words_frequency;
    double static words_speed;
    double static words_size;
    bool static words_highlight;

    float static ui_scale;

    inline bool static settingsPanelUpToDate = true; // when some e.g. textFields from settings panel change their values its set to false;
public:
    bool static isSettingsPanelUpToDate();
    void static setUpToDateValue(bool value);

    void static loadSettings();
    void static saveSettings();

    void static restoreDefaultSettings();
    void static applySettingsPanel();

    void static setWordsFontName(std::string const& value);
    std::string static getWordsFontName();

    void static setWordsFrequency(std::string const& value);
    void static setWordsFrequency(double value);
    double static getWordsFrequency();

    void static setWordsSpeed(std::string const& value);
    void static setWordsSpeed(double value);
    double static getWordsSpeed();

    void static setWordsSize(std::string const& value);
    void static setWordsSize(double value);
    double static getWordsSize();

    void static setWordsHighlight(std::string const& value);
    void static setWordsHighlight(bool value);
    bool static isWordsHighlightEnabled();

    float static getUIScale();
    void static setUIScale(std::string const& value);
    void static setUIScale(float value);

    std::string static buildEndGameSettings();

// Game End Criterias (no setters and getters - too much code)
    inline bool static endGame_missedWords_bool = true;
    inline bool static endGame_time_bool = false;
    inline bool static endGame_score_bool = false;

    inline int static endGame_missedWords_value = 5;
    inline auto static endGame_time_value = std::chrono::duration<double>(10);
    inline int static endGame_score_value = 20;
};
