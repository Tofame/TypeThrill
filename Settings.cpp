#include "Settings.h"

#include "Globals.h"

#include <fstream>
#include <regex>
#include <fmt/core.h>
#include <iostream>

#include "ResourceManagers/FontManager.h"

// Settings's default values that we use when malformed settings or couldn't load one
std::string defaultWordFontName = "arial";
double defaultWordFrequency = 1.1;
double defaultWordSpeed = 0.4;
double defaultWordSize = 0.4;

float defaultUIScale = 1.0;

std::string Settings::words_fontName = defaultWordFontName;
double Settings::words_frequency = defaultWordFrequency;
double Settings::words_speed = defaultWordSpeed;
double Settings::words_size = defaultWordSize;
bool Settings::words_highlight = true;

float Settings::ui_scale = defaultUIScale;

void Settings::loadSettings() {
    auto file = std::fstream(projectPath + "/settings.txt");

    if (!file) {
        return;
    }

    std::regex pattern("([a-zA-Z_]+)[=]{1}(.+)");
    std::smatch matches;

    for (auto line = std::string(); std::getline(file, line); ) {
        if(line.empty() || line[0] == '/')
            continue;

        if (std::regex_match(line, matches, pattern)) {
            std::string option = matches[1].str();
            std::string value = matches[2].str();

            // Switch does not support strings, so we use elseif.
            if (option == "words_font") {
                Settings::setWordsFontName(value);
            } else if (option == "words_frequency") {
                Settings::setWordsFrequency(value);
            } else if (option == "words_speed") {
                Settings::setWordsSpeed(value);
            } else if (option == "words_size") {
                Settings::setWordsSize(value);
            } else if (option == "words_highlight") {
                Settings::setWordsHighlight(value);
            } else if (option == "ui_scale") {
                Settings::setUIScale(value);
            } else {
                fmt::println("Unknown settings.txt option: {}.\nThe full line is: {}", option, matches[0].str());
            }
        } else {
            throw std::runtime_error("Malformed settings.txt line: " + line);
        }
    }
}

void Settings::setWordsFontName(std::string const& value) {
    if(FontManager::Fonts.contains(value)) {
        Settings::words_fontName = value;
    } else {
        Settings::words_fontName = defaultWordFontName;
    }
}

void Settings::setWordsFrequency(std::string const& value) {
    try {
        Settings::words_frequency = std::stod(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_frequency = defaultWordFrequency;
    }
}

void Settings::setWordsSpeed(std::string const& value) {
    try {
        Settings::words_speed = std::stod(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_speed = defaultWordSpeed;
    }
}

void Settings::setWordsSize(std::string const& value) {
    try {
        Settings::words_size = std::stod(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_size = defaultWordSize;
    }
}

void Settings::setWordsHighlight(std::string const& value) {
    Settings::words_highlight = (value == "true" || value == "True");
}

void Settings::setUIScale(std::string const& value) {
    try {
        Settings::ui_scale = std::stof(value);
    } catch (const std::invalid_argument& e) {
        Settings::ui_scale = defaultUIScale;
    }
}

std::string Settings::getWordsFontName() {
    return Settings::words_fontName;
}

double Settings::getWordsFrequency() {
    return Settings::words_frequency;
}

double Settings::getWordsSpeed() {
    return Settings::words_speed;
}

double Settings::getWordsSize() {
    return Settings::words_size;
}

bool Settings::isWordsHighlightEnabled() {
    return Settings::words_highlight;
}

float Settings::getUIScale() {
    return Settings::ui_scale;
}