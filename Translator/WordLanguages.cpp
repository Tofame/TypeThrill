#include "WordLanguages.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <random>

#include "../Globals.h"
#include "fmt/Core.h"

int getRandomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

std::map<std::string, std::vector<std::wstring>> WordLanguages::wordsLocales = std::map<std::string, std::vector<std::wstring>>();

void WordLanguages::loadLocales() {
    auto directory = projectPath + "/Resources/Locales";

    // Rest of the project supports special characters like "ć" etc. as it was achieved with using sf::String etc.
    // However while loading locales it seems default C++ doesn't have anything other than :alpha: which doesnt include them
    // Note: ask professor if \p{L} exists and how can I use it (is it in boost?)
    std::wregex wordRegex(L"^([[:alpha:] ])+$"); // Should accept any alphabet, special letters too (like ć)

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            std::string filename = entry.path().stem().string();
            auto file = std::wfstream(entry.path());

            // https://en.cppreference.com/w/cpp/io/basic_fstream/is_open
            // Checks if the file stream has an associated file
            // We loop through entries so they do have to exist, but we can still make sure
            if (file.is_open()) {
                auto words = std::vector<std::wstring>();

                std::wstring line;
                while (std::getline(file, line)) {
                    if (!line.empty() && std::regex_match(line, wordRegex)) {
                        words.push_back(line);
                    }
                }

                file.close();
                WordLanguages::wordsLocales[filename] = words;
            } else {
                throw std::runtime_error("Failed to open file: " + filename);
            }
        }
    }
}

std::wstring WordLanguages::getRandomWord() {
    // Needs to be swapped for getLanguage()
    std::string language = "english";

    try {
        auto words = WordLanguages::wordsLocales.at(language);
        //fmt::println("{} word", std::to_string(wordsLocales[L"polish"][0]));
        if (!words.empty()) {
            return words[getRandomInt(0, words.size() - 1)];
        } else {
            throw std::runtime_error("Locales for language: " + language + " has no words.");
        }
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("There is no locales for language: " + language);
    }
}