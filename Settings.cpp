#include "Settings.h"

#include "Globals.h"

#include <fstream>
#include <regex>
#include <fmt/core.h>
#include <iostream>

#include "Game/GameInterface.h"
#include "Game/GameStatistics.h"
#include "ResourceManagers/FontManager.h"
#include "UI/Checkbox.h"
#include "UI/ComboBox.h"
#include "UI/TextField.h"

void restoreDefaultTextFields(TextField* txtFieldPtr);
void setSettingFromTextField(TextField* txtFieldPtr);

void restoreDefaultCheckboxes(Checkbox* checkBoxPtr);

void restoreDefaultComboBoxes(ComboBox* comboBoxPtr);
void setSettingFromComboBoxes(ComboBox* comboBoxPtr);

// resetDefaultSettings also could be defined as "bool firstTimePreLoaded" (to give you better grasp of what is it for)
void Settings::preLoadSettings(bool resetDefaultSettings) {
    if(resetDefaultSettings) {
        defaultSettingsMap["words_fontName"] = "aleoRegular";
        defaultSettingsMap["words_frequency"] = "2.1";
        defaultSettingsMap["words_speed"] = "0.001";
        defaultSettingsMap["words_size"] = "1.0";
        defaultSettingsMap["words_highlight"] = "true";

        defaultSettingsMap["ui_scale"] = "1.0";

        defaultSettingsMap["endGame_missedWords_bool"] = "true";
        defaultSettingsMap["endGame_time_bool"] = "false";
        defaultSettingsMap["endGame_score_bool"] = "false";
        defaultSettingsMap["endGame_never_bool"] = "false";

        defaultSettingsMap["endGame_missedWords_value"] = "5";
        defaultSettingsMap["endGame_time_value"] = "60";
        defaultSettingsMap["endGame_score_value"] = "20";

        defaultSettingsMap["word_locale"] = "english";
    }

    settingsMap = defaultSettingsMap;
}

void Settings::loadSettings() {
    auto file = std::fstream(projectPath + "/settings.txt");

    if (!file) {
        throw std::runtime_error("Unable to open settings.txt.");
    }

    std::regex pattern("([a-zA-Z_]+)[=]{1}(.+)");
    std::smatch matches;

    for (auto line = std::string(); std::getline(file, line); ) {
        if(line.empty() || line[0] == '/')
            continue;

        if (std::regex_match(line, matches, pattern)) {
            std::string option = matches[1].str();
            std::string value = matches[2].str();

            if (settingsMap.contains(option)) {
                settingsMap[option] = value;
            } else {
                throw std::runtime_error(fmt::format("Settings.txt option: {} does not exist in settingsMap.\nThe full line is: {}", option, matches[0].str()));
            }
        } else {
            throw std::runtime_error("Malformed settings.txt line: " + line);
        }
    }
}

void Settings::restoreDefaultSettings() {
    // Reset values in TextField(s) etc.
    auto settingsPanel = GameInterface::getPanelByType(PANEL_SETTINGS);
    for(auto uielement : settingsPanel->UIElements) {
        // https://en.cppreference.com/w/cpp/language/dynamic_cast
        // Safely converts pointers and references to classes up, down, and sideways along the inheritance hierarchy.
        // Im using static_cast though, as its cheaper and I am sure uielement is parent class
        switch(uielement->getType()) {
            case TEXTFIELD:
            {
                auto txtFieldPtr = static_cast<TextField*>(uielement);
                restoreDefaultTextFields(txtFieldPtr);
                break;
            }
            case CHECKBOX:
            {
                auto checkBoxPtr = static_cast<Checkbox*>(uielement);
                restoreDefaultCheckboxes(checkBoxPtr);
                break;
            }
            case COMBOBOX:
            {
                auto comboBoxPtr = static_cast<ComboBox*>(uielement);
                restoreDefaultComboBoxes(comboBoxPtr);
                break;
            }
            default:
                break;
        }
    }

    // This basically will reset all the keys in the map (meaning it will reset settings)
    Settings::preLoadSettings(false);

    // Saves settings to .txt
    Settings::saveSettings();
}

void Settings::saveSettingsPanel() {
    // Update DynamicTextLabel in GameStatistics
    auto gameStatisticsPanel = GameInterface::getPanelByType(PANEL_GAMESTATISTICS);
    if(gameStatisticsPanel == nullptr) {
        throw std::runtime_error("Settings::saveSettingsPanel() can't seem to find PANEL_GAMESTATISTICS. Does it exist?");
    }

    for(auto uielement : gameStatisticsPanel->UIElements) {
        if(uielement->getType() == DYNAMICTEXTLABEL) {
            uielement->update();
        }
    }

    // Saves settings to .txt
    Settings::saveSettings();
}

void Settings::setWordsFontName(std::string const& value) {
    if(FontManager::Fonts.contains(value)) {
        settingsMap["words_fontName"] = value;
    } else {
        settingsMap["words_fontName"] = defaultSettingsMap["words_fontName"];
    }
}

void Settings::setWordsFrequency(std::string const& value) {
    try {
        // We do this to make sure the string we will save to map is of good type
        // for frequency the string must be convertable to double
        auto tempValue = std::stod(value);
        settingsMap["words_frequency"] = value;
    } catch (const std::invalid_argument& e) {
        settingsMap["words_frequency"] = defaultSettingsMap["words_frequency"];
    }
}

void Settings::setWordsSpeed(std::string const& value) {
    try {
        auto tempValue = std::stod(value);
        settingsMap["words_speed"] = value;
    } catch (const std::invalid_argument& e) {
        settingsMap["words_speed"] = defaultSettingsMap["words_speed"];
    }
}

void Settings::setWordsSize(std::string const& value) {
    try {
        auto tempValue = std::stod(value);
        settingsMap["words_size"] = value;
    } catch (const std::invalid_argument& e) {
        settingsMap["words_size"] = defaultSettingsMap["words_size"];
    }
}

void Settings::setWordsHighlight(std::string const& value) {
    settingsMap["words_highlight"] = value;
}

void Settings::setWordsHighlight(bool const &value) {
    if(value == true) {
        settingsMap["words_highlight"] = "true";
    } else {
        settingsMap["words_highlight"] = "false";
    }
}

void Settings::setUIScale(std::string const& value) {
    try {
        auto tempValue = std::stof(value);
        if(tempValue > 0) {
            settingsMap["ui_scale"] = value;
        } else { // Failsafe from setting ui_scale to 0 (or less) which means UI would disappear
            settingsMap["ui_scale"] = 0.1;
        }
    } catch (const std::invalid_argument& e) {
        settingsMap["ui_scale"] = defaultSettingsMap["ui_scale"];
    }

    // Refresh panel sizes
    for(auto panel : GameInterface::panels) {
        panel->update();
    }
    GameInterface::updateGameTitle();
}

std::string Settings::getWordsFontName(bool defaultValue) {
    if(defaultValue == true)
        return defaultSettingsMap["words_fontName"];

    return settingsMap["words_fontName"];
}

double Settings::getWordsFrequency(bool defaultValue) {
    if(defaultValue == true)
        return std::stod(defaultSettingsMap["words_frequency"]);

    auto value = 0.0;
    try {
        value = std::stod(settingsMap["words_frequency"]);
    } catch (const std::invalid_argument& e) {
        value = std::stod(defaultSettingsMap["words_frequency"]);
    }

    return value;
}

double Settings::getWordsSpeed(bool defaultValue) {
    if(defaultValue == true)
        return std::stod(defaultSettingsMap["words_speed"]);

    auto value = 0.0;
    try {
        value = std::stod(settingsMap["words_speed"]);
    } catch (const std::invalid_argument& e) {
        value = std::stod(defaultSettingsMap["words_speed"]);
    }

    return value;
}

double Settings::getWordsSize(bool defaultValue) {
    if(defaultValue == true)
        return std::stod(defaultSettingsMap["words_size"]);

    auto value = 0.0;
    try {
        value = std::stod(settingsMap["words_size"]);
    } catch (const std::invalid_argument& e) {
        value = std::stod(defaultSettingsMap["words_size"]);
    }

    return value;
}

bool Settings::isWordsHighlightEnabled(bool defaultValue) {\
    std::string value = "true";

    if(defaultValue)
        value = defaultSettingsMap["words_highlight"];
    else
        value = settingsMap["words_highlight"];

    return value == "true" || value == "True";
}

float Settings::getUIScale(bool defaultValue) {
    if(defaultValue == true)
        return std::stof(defaultSettingsMap["ui_scale"]);

    auto value = 0.0f;
    try {
        value = std::stof(settingsMap["ui_scale"]);
    } catch (const std::invalid_argument& e) {
        value = std::stof(defaultSettingsMap["ui_scale"]);
    }

    return value;
}

void restoreDefaultTextFields(TextField* txtFieldPtr) {
    std::string text = txtFieldPtr->getText().getString();
    if (text.starts_with("Word Speed"))
        txtFieldPtr->setInput(fmt::format("{:.5f}", Settings::getWordsSpeed(true)));
    else if (text.starts_with("Word Frequency"))
        txtFieldPtr->setInput(fmt::format("{:.2f}", Settings::getWordsFrequency(true)));
    else if (text.starts_with("Word Size"))
        txtFieldPtr->setInput(fmt::format("{:.2f}", Settings::getWordsSize(true)));
    else if (text.starts_with("UI Scale"))
        txtFieldPtr->setInput(fmt::format("{:.2f}", Settings::getUIScale(true)));

    txtFieldPtr->update();
}

void restoreDefaultCheckboxes(Checkbox* checkBoxPtr) {
    std::string text = checkBoxPtr->getText().getString();
    if (text.starts_with("Word Highlight")) {
        Settings::isWordsHighlightEnabled(true) == true ? checkBoxPtr->check() : checkBoxPtr->uncheck();
    }
}

void restoreDefaultComboBoxes(ComboBox* comboBoxPtr) {
    std::string text = comboBoxPtr->getText().getString();
    if (text.starts_with("Word Font")) {
        comboBoxPtr->setChosenText(fmt::format("{}", Settings::getWordsFontName(true)));
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
    else if (text.starts_with("UI Scale")) {
        Settings::setUIScale(value);
        GameInterface::updatePanels();
        GameInterface::updateGameTitle();
    }
}

void setSettingFromComboBoxes(ComboBox* comboBoxPtr) {
    std::string text = comboBoxPtr->getText().getString();

    if (text.starts_with("Word Font")) {
        Settings::setWordsFontName(comboBoxPtr->getChosenTextString());
    }
}

void Settings::saveSettings() {
    // Allow for both reading AND writing
    auto file = std::fstream(projectPath + "/settings.txt", std::ios::in | std::ios::out);

    if (!file) {
        throw std::runtime_error("Unable to open settings.txt.");
    }

    std::regex pattern("([a-zA-Z_]+)[=]{1}(.+)");
    std::smatch matches;
    std::stringstream buffer;

    for (std::string line; std::getline(file, line); ) {
        if (std::regex_match(line, matches, pattern)) {
            std::string option = matches[1].str();
            std::string newValue;

            if (option == "words_fontName") {
                newValue = Settings::getWordsFontName(false);
            } else if (option == "words_frequency") {
                newValue = fmt::format("{:.2f}", Settings::getWordsFrequency(false));
            } else if (option == "words_speed") {
                newValue = fmt::format("{:.5f}", Settings::getWordsSpeed(false));
            } else if (option == "words_size") {
                newValue = fmt::format("{:.2f}", Settings::getWordsSize(false));
            } else if (option == "words_highlight") {
                newValue = Settings::isWordsHighlightEnabled(false) ? "true" : "false";
            } else if (option == "ui_scale") {
                newValue = fmt::format("{:.2f}", Settings::getUIScale(false));
            }

            buffer << option << "=" << newValue << "\n";
        } else {
            buffer << line << "\n";
        }
    }

    // Close and then update file
    file.close();
    std::ofstream outFile(projectPath + "/settings.txt");
    outFile << buffer.str();
    outFile.close();
}

std::string Settings::buildEndGameSettings() {
    std::string result;
    Settings::setEndGameCriteriumBool("endGame_never_bool", true);

    if (Settings::getEndGameCriteriumBool("endGame_missedWords_bool") == true) {
        Settings::setEndGameCriteriumBool("endGame_never_bool", false);
        result += fmt::format("Ends on:\t{} missed\t", getEndGameCriterium_missedWords());
    }

    if (Settings::getEndGameCriteriumBool("endGame_score_bool") == true) {
        Settings::setEndGameCriteriumBool("endGame_never_bool", false);
        result += fmt::format("Ends on:\t{} score\t", getEndGameCriterium_score());
    }

    if (Settings::getEndGameCriteriumBool("endGame_time_bool") == true) {
        Settings::setEndGameCriteriumBool("endGame_never_bool", false);
        result += fmt::format("Ends on:\t{} time\t", GameStatistics::getInstance()->formatTime(getEndGameCriterium_time()));
    }

    if(Settings::getEndGameCriteriumBool("endGame_never_bool") == true) {
        result += fmt::format("This game never ends");
    }

    return result;
}

void Settings::setEndGameCriteriumBool(std::string const& criterium, bool value) {
    if(value == true) {
        settingsMap[criterium] = "true";
    } else {
        settingsMap[criterium] = "false";
    }
}

bool Settings::getEndGameCriteriumBool(std::string const& criterium) {
    auto value = settingsMap[criterium];
    return value == "true" || value == "True";
}

void Settings::setEndGameCriterium_missedWords(int value) {
    settingsMap["endGame_missedWords_value"] = std::to_string(value);
}

void Settings::setEndGameCriterium_missedWords(std::string value) {
    settingsMap["endGame_missedWords_value"] = value;
}

int Settings::getEndGameCriterium_missedWords() {
    return std::stoi(settingsMap["endGame_missedWords_value"]);
}

void Settings::setEndGameCriterium_time(double value) {
    settingsMap["endGame_time_value"] = std::to_string(value);
}

void Settings::setEndGameCriterium_time(std::string value) {
    settingsMap["endGame_time_value"] = value;
}

std::chrono::duration<double> Settings::getEndGameCriterium_time() {
    return std::chrono::duration<double>(std::stod(settingsMap["endGame_time_value"]));
}

void Settings::setEndGameCriterium_score(int value) {
    settingsMap["endGame_score_value"] = std::to_string(value);
}

void Settings::setEndGameCriterium_score(std::string value) {
    settingsMap["endGame_score_value"] = value;
}

int Settings::getEndGameCriterium_score() {
    return std::stoi(settingsMap["endGame_score_value"]);
}

void Settings::setWordLocale(std::string &value) {
    settingsMap["word_locale"] = value;
}

std::string Settings::getWordLocale(bool defaultValue) {
    if(defaultValue) {
        return defaultSettingsMap["word_locale"];
    }

    return settingsMap["word_locale"];
}