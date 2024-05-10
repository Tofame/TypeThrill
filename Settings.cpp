#include "Settings.h"

#include "Globals.h"

#include <fstream>
#include <regex>
#include <fmt/core.h>
#include <iostream>

int Settings::words_fontId;
double Settings::words_frequency;
double Settings::words_speed;
double Settings::words_size;
bool Settings::words_highlight;

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
                Settings::setWordFontId(value);
            } else if (option == "words_frequency") {
                Settings::setWordFrequency(value);
            } else if (option == "words_speed") {
                Settings::setWordSpeed(value);
            } else if (option == "words_size") {
                Settings::setWordSize(value);
            } else if (option == "words_highlight") {
                Settings::setWordHighlight(value);
            } else {
                fmt::println("Unknown settings.txt option: {}.\\nThe full line is: {}", option, matches[0].str());
            }
        } else {
            throw std::runtime_error("Malformed settings.txt line: " + line);
        }
    }
}

void Settings::setWordFontId(std::string const& value) {
    try {
        Settings::words_fontId = std::stoi(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_fontId = 1;
    }
}

void Settings::setWordFrequency(std::string const& value) {
    try {
        Settings::words_frequency = std::stod(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_frequency = 1.1;
    }
}

void Settings::setWordSpeed(std::string const& value) {
    try {
        Settings::words_speed = std::stod(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_speed = 0.4;
    }
}

void Settings::setWordSize(std::string const& value) {
    try {
        Settings::words_size = std::stod(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_size = 2.0;
    }
}

void Settings::setWordHighlight(std::string const& value) {
    Settings::words_highlight = (value == "true" || value == "True");
}

int Settings::getWordFontId() {
    return Settings::words_fontId;
}

double Settings::getWordFrequency() {
    return Settings::words_frequency;
}

double Settings::getWordSpeed() {
    return Settings::words_speed;
}

double Settings::getWordSize() {
    return Settings::words_size;
}

bool Settings::isWordHighlightEnabled() {
    return Settings::words_highlight;
}