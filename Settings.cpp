#include "Settings.h"

#include "Globals.h"

#include <fstream>
#include <regex>
#include <fmt/core.h>
#include <iostream>

int Settings::words_fontId = 1;
double Settings::word_frequency = 1.1;
double Settings::words_speed = 0.4;
double Settings::words_size = 2.0;
bool Settings::words_highlight = true;

void Settings::loadSettings() {
    auto file = std::fstream(projectPath + "/settings.txt");

    if (!file) {
        return;
    }

    for (auto line = std::string(); std::getline(file, line); ) {
        if(line.empty() || line[0] == '/')
            continue;

        std::regex pattern("([a-zA-Z_]+)[=]{1}(.+)");
        std::smatch matches;

        if (std::regex_match(line, matches, pattern)) {
            std::string option = matches[1].str();
            std::string value = matches[2].str();

            // Switch does not support strings, so we use elseif.
            if (option == "words_font") {
                Settings::words_fontId = std::stoi(value);
            } else if (option == "words_frequency") {
                Settings::word_frequency = std::stod(value);
            } else if (option == "words_speed") {
                Settings::words_speed = std::stod(value);
            } else if (option == "words_size") {
                Settings::words_size = std::stod(value);
            } else if (option == "words_highlight") {
                Settings::words_highlight = (value == "true");
            } else {
                fmt::println("Unknown settings.txt option: {}.\\nThe full line is: ", option, matches[0].str());
            }
        } else {
            throw std::runtime_error("Malformed settings.txt line: " + line);
        }
    }
}
