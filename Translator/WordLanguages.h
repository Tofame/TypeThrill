#pragma once

#include <map>
#include <string>
#include <vector>

class WordLanguages {
private:
    static std::map<std::string, std::vector<std::string>> wordsLocales;

public:
    static void loadLocales();
    static std::string getRandomWord();
};
