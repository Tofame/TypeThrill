#include "WordLocales.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <random>

#include "../Globals.h"
#include "../Settings.h"
#include "fmt/Core.h"

int getRandomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

std::map<std::string, std::vector<std::string>> WordLocales::wordsLocales = std::map<std::string, std::vector<std::string>>();

void WordLocales::loadLocales() {
    auto directory = projectPath + "/Resources/Locales";

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            std::string filename = entry.path().stem().string();
            auto file = std::fstream(entry.path());

            // https://en.cppreference.com/w/cpp/io/basic_fstream/is_open
            // Checks if the file stream has an associated file
            // We loop through entries so they do have to exist, but we can still make sure
            if (file.is_open()) {
                auto words = std::vector<std::string>();

                std::string line;
                while (std::getline(file, line)) {
                    if (!line.empty()) {
                        words.push_back(line);
                    }
                }

                file.close();
                WordLocales::wordsLocales[filename] = words;
            } else {
                throw std::runtime_error("Failed to open file: " + filename);
            }
        }
    }
}

std::string& WordLocales::getRandomWord() {
    std::string locale = Settings::getInstance()->getWordLocale(false);

    try {
        auto words = WordLocales::wordsLocales.at(locale);

        if (!words.empty()) {
            return words[getRandomInt(0, words.size() - 1)];
        } else {
            throw std::runtime_error("Locales for words: " + locale + " is empty (no words).");
        }
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("There is no words locale such as: " + locale);
    }
}

void WordLocales::validateLocale(UIElement* confirmButton, TextField* textfield) {
    confirmButton->body.setFillColor(UIElementFactory::ColorBlue);

    if(wordsLocales.contains(textfield->getInputString()) == false) {
        auto defaultLocale = Settings::getInstance()->getWordLocale(true);
        Settings::getInstance()->setWordLocale(defaultLocale);
        textfield->setInput(defaultLocale);
        textfield->update();
        return;
    }

    auto localeString = textfield->getInputString().toAnsiString();
    Settings::getInstance()->setWordLocale(localeString);
}