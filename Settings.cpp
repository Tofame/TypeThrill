#include "Settings.h"

#include "Globals.h"

#include <fstream>
#include <regex>
#include <fmt/core.h>
#include <iostream>

#include "Game/GameInterface.h"
#include "ResourceManagers/FontManager.h"
#include "UI/Checkbox.h"
#include "UI/TextField.h"

void restoreDefaultTextFields(TextField* txtFieldPtr);
void setSettingFromTextField(TextField* txtFieldPtr);

void restoreDefaultCheckboxes(Checkbox* checkBoxPtr);
void setSettingFromCheckbox(Checkbox* checkBoxPtr);
// Settings's default values that we use when malformed settings or couldn't load one
std::string defaultWordFontName = "arial";
double defaultWordFrequency = 1.1;
double defaultWordSpeed = 0.01;
double defaultWordSize = 1.0;
double defaultWordHighlight = true;

float defaultUIScale = 1.0;

std::string Settings::words_fontName = defaultWordFontName;
double Settings::words_frequency = defaultWordFrequency;
double Settings::words_speed = defaultWordSpeed;
double Settings::words_size = defaultWordSize;
bool Settings::words_highlight = true;

float Settings::ui_scale = defaultUIScale;


bool Settings::isSettingsPanelUpToDate() {
    return settingsPanelUpToDate == true;
}

void Settings::setUpToDateValue(bool value) {
    settingsPanelUpToDate = value;
}

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

void Settings::restoreDefaultSettings() {
    auto settingsPanel = GameInterface::getPanelByType(PANEL_SETTINGS);
    for(auto uielement : settingsPanel->UIElements) {
        // https://en.cppreference.com/w/cpp/language/dynamic_cast
        // Safely converts pointers and references to classes up, down, and sideways along the inheritance hierarchy.
        auto txtFieldPtr = dynamic_cast<TextField*>(uielement);
        if (txtFieldPtr) {
            restoreDefaultTextFields(txtFieldPtr);
            continue;
        }

        auto checkBoxPtr = dynamic_cast<Checkbox*>(uielement);
        if (checkBoxPtr) {
            restoreDefaultCheckboxes(checkBoxPtr);
            continue;
        }
    }

    setWordsFrequency(defaultWordFrequency);
    setWordsSize(defaultWordSize);
    setWordsSpeed(defaultWordSpeed);
    setWordsHighlight(defaultWordHighlight);

    setUIScale(defaultUIScale);
}

void Settings::applySettingsPanel() {
    auto settingsPanel = GameInterface::getPanelByType(PANEL_SETTINGS);
    for(auto uielement : settingsPanel->UIElements) {
        // https://en.cppreference.com/w/cpp/language/dynamic_cast
        // Safely converts pointers and references to classes up, down, and sideways along the inheritance hierarchy.
        auto txtFieldPtr = dynamic_cast<TextField*>(uielement);
        if (txtFieldPtr) {
            setSettingFromTextField(txtFieldPtr);
            continue;
        }

        auto checkBoxPtr = dynamic_cast<Checkbox*>(uielement);
        if (checkBoxPtr) {
            setSettingFromCheckbox(checkBoxPtr);
            continue;
        }
    }

    Settings::saveSettings();
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

void Settings::setWordsFrequency(double value) {
    Settings::words_frequency = value;
}

void Settings::setWordsSpeed(std::string const& value) {
    try {
        Settings::words_speed = std::stod(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_speed = defaultWordSpeed;
    }
}

void Settings::setWordsSpeed(double value) {
    Settings::words_speed = value;
}

void Settings::setWordsSize(std::string const& value) {
    try {
        Settings::words_size = std::stod(value);
    } catch (const std::invalid_argument& e) {
        Settings::words_size = defaultWordSize;
    }
}

void Settings::setWordsSize(double value) {
    Settings::words_size = value;
}

void Settings::setWordsHighlight(std::string const& value) {
    Settings::words_highlight = (value == "true" || value == "True");
}

void Settings::setWordsHighlight(bool value) {
    Settings::words_highlight = value;
}

void Settings::setUIScale(std::string const& value) {
    try {
        Settings::ui_scale = std::stof(value);
    } catch (const std::invalid_argument& e) {
        Settings::ui_scale = defaultUIScale;
    }
}

void Settings::setUIScale(float value) {
    Settings::ui_scale = value;
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

void restoreDefaultTextFields(TextField* txtFieldPtr) {
    std::string text = txtFieldPtr->getText().getString();
    if (text.starts_with("Word Speed"))
        txtFieldPtr->setInput(fmt::format("{:.2f}", defaultWordSpeed));
    else if (text.starts_with("Word Frequency"))
        txtFieldPtr->setInput(fmt::format("{:.2f}", defaultWordFrequency));
    else if (text.starts_with("Word Size"))
        txtFieldPtr->setInput(fmt::format("{:.2f}", defaultWordSize));
    else if (text.starts_with("UI Scale"))
        txtFieldPtr->setInput(fmt::format("{:.2f}", defaultUIScale));

    txtFieldPtr->update();
}

void restoreDefaultCheckboxes(Checkbox* checkBoxPtr) {
    std::string text = checkBoxPtr->getText().getString();
    if (text.starts_with("Word Highlight")) {
        defaultWordHighlight == true ? checkBoxPtr->enable() : checkBoxPtr->disable();
    }
}

void setSettingFromTextField(TextField* txtFieldPtr) {
    std::string text = txtFieldPtr->getText().getString();
    auto value = txtFieldPtr->getInputString();

    if (text.starts_with("Word Speed"))
        Settings::setWordsSpeed(value);
    else if (text.starts_with("Word Frequency"))
        Settings::setWordsFrequency(value);
    else if (text.starts_with("Word Size"))
        Settings::setWordsSize(value);
    else if (text.starts_with("UI Scale"))
        Settings::setUIScale(value);
}

void setSettingFromCheckbox(Checkbox* checkBoxPtr) {
    std::string text = checkBoxPtr->getText().getString();
    bool value = checkBoxPtr->isEnabled() == true;

    if (text.starts_with("Word Highlight")) {
        Settings::setWordsHighlight(value);
    }
}

void Settings::saveSettings() {
    
}