#pragma once

struct Settings {
    int static words_fontId;
    double static word_frequency;
    double static words_speed;
    double static words_size;
    bool static words_highlight;

    void static loadSettings();
};