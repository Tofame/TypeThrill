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
                Settings::setWordsFontId(value);
            } else if (option == "words_frequency") {
                Settings::setWordsFrequency(value);
            } else if (option == "words_speed") {
                Settings::setWordsSpeed(value);
            } else if (option == "words_size") {
                Settings::setWordsSize(value);
            } else if (option == "words_highlight") {
                Settings::setWordsHighlight(value);
            } else {
                fmt::println("Unknown settings.txt option: {}.\\nThe full line is: {}", option, matches[0].str());
            }
        } else {
            throw std::runtime_error("Malformed settings.txt line: " + line);
        }
    }
}

void Settings::setWordsFontId(std::string const& value) {
    try {
        Settings::words_fontId = std::stoi(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_fontId = 1;
    }
}

void Settings::setWordsFrequency(std::string const& value) {
    try {
        Settings::words_frequency = std::stod(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_frequency = 1.1;
    }
}

void Settings::setWordsSpeed(std::string const& value) {
    try {
        Settings::words_speed = std::stod(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_speed = 0.4;
    }
}

void Settings::setWordsSize(std::string const& value) {
    try {
        Settings::words_size = std::stod(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_size = 2.0;
    }
}

void Settings::setWordsHighlight(std::string const& value) {
    Settings::words_highlight = (value == "true" || value == "True");
}

int Settings::getWordsFontId() {
    return Settings::words_fontId;
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