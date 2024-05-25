#pragma once
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
};
