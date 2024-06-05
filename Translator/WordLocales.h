#pragma once

#include <map>
#include <string>
#include <vector>

#include "../UI/UIElement.h"
#include "../UI/UIElementFactory.h"

class WordLocales {
private:
    static std::map<std::string, std::vector<std::string>> wordsLocales;

public:
    static void loadLocales();
    static std::string& getRandomWord();

    static void validateLocale(UIElement* confirmButton, TextField* textfield);
};