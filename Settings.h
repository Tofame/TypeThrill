#pragma once
#include <string>

class Settings {
private:
    int static words_fontId;
    double static words_frequency;
    double static words_speed;
    double static words_size;
    bool static words_highlight;

    float static ui_scale;
public:
    void static loadSettings();

    void static setWordsFontId(std::string const& value);
    int static getWordsFontId();
    void static setWordsFrequency(std::string const& value);
    double static getWordsFrequency();
    void static setWordsSpeed(std::string const& value);
    double static getWordsSpeed();
    void static setWordsSize(std::string const& value);
    double static getWordsSize();
    void static setWordsHighlight(std::string const& value);
    bool static isWordsHighlightEnabled();

    float static getUIScale();
    void static setUIScale(std::string const& value);
};
