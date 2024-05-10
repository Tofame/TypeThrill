#pragma once
#include <string>

class Settings {
private:
    int static words_fontId;
    double static words_frequency;
    double static words_speed;
    double static words_size;
    bool static words_highlight;
public:
    void static loadSettings();

    void static setWordsFontId(std::string const& value);
    void static setWordsFrequency(std::string const& value);
    void static setWordsSpeed(std::string const& value);
    void static setWordsSize(std::string const& value);
    void static setWordsHighlight(std::string const& value);

    int static getWordsFontId();
    double static getWordsFrequency();
    double static getWordsSpeed();
    double static getWordsSize();
    bool static isWordsHighlightEnabled();
};
