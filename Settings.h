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

    void static setWordFontId(std::string const& value);
    void static setWordFrequency(std::string const& value);
    void static setWordSpeed(std::string const& value);
    void static setWordSize(std::string const& value);
    void static setWordHighlight(std::string const& value);

    int static getWordFontId();
    double static getWordFrequency();
    double static getWordSpeed();
    double static getWordSize();
    bool static isWordHighlightEnabled();
};
