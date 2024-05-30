#pragma once
#include "Button.h"
#include "Checkbox.h"
#include "Panel.h"
#include "TextField.h"
#include "ComboBox.h"
#include "UIElement.h"
#include "DynamicTextLabel.h"
#include "../Game/GameInterface.h"
#include "../Settings.h"
#include "../Globals.h"
#include "../ResourceManagers/FontManager.h"

inline auto ColorBlue = sf::Color(52, 194, 199);
inline auto ColorGray = sf::Color(105,104,102);
inline auto ColorGrayDarkLO = sf::Color(64,64,64, 120);
inline auto ColorGrayDark = sf::Color(54, 54, 54);

class UIElementFactory {
public:
    static ComboBox* createComboBox(const std::string& fieldValue, sf::Vector2f posRatios, const std::string& textValue) {
        sf::Vector2f size = {200, 30};
        auto element = new ComboBox(size, posRatios);

        auto& rect = element->body;
        rect.setFillColor(ColorGrayDark);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(3);

        if(textValue.empty() == false) {
            auto text = sf::Text();
            text.setFont(FontManager::Fonts["times"]);
            text.setCharacterSize(GameInterface::smallCharacterSize);

            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(1);

            text.setString(textValue);

            element->setText(text);
        }

        if(fieldValue.empty() == false) {
            auto chosenText = sf::Text();
            chosenText.setFont(FontManager::Fonts["aleoItalic"]);
            chosenText.setCharacterSize(GameInterface::smallCharacterSize);
            chosenText.setFillColor(sf::Color::White);
            chosenText.setString(fieldValue);

            element->setChosenText(chosenText);
        }

        return element;
    }

    static DynamicTextLabel* createStatisticsDynamicLabel(sf::Vector2f ratios, std::function<std::string()> const& onUpdateString) {
        auto element = new DynamicTextLabel(ratios, onUpdateString);

        element->getText().setCharacterSize(GameInterface::mediumLiteCharacterSize);
        element->setAlignType(ALIGN_NONE);

        return element;
    }

    static DynamicTextLabel* createInfoDynamicLabel(sf::Vector2f ratios, std::function<std::string()> const& onUpdateString) {
        auto element = new DynamicTextLabel(ratios, onUpdateString);

        element->getText().setCharacterSize(GameInterface::smallCharacterSize);
        element->setAlignType(ALIGN_NONE);

        return element;
    }

    static Checkbox* createMenuCheckbox(float sizeMultiplier, sf::Vector2f posRatios, std::string textValue, bool defaultEnabled, std::function<void(bool value)> const& onCheckboxUpdate) {
        auto element = new Checkbox(sizeMultiplier, posRatios, defaultEnabled);

        element->onCheckboxUpdate = onCheckboxUpdate;

        if(textValue.empty() == false) {
            auto text = sf::Text();
            text.setFont(FontManager::Fonts["times"]);
            text.setCharacterSize(GameInterface::smallCharacterSize);

            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(1);

            text.setString(textValue);

            element->setText(text);
        }

        return element;
    }

    static TextField* createTextField(const std::string& fieldValue, sf::Vector2f posRatios, std::string textValue, std::wstring pattern) {
        sf::Vector2f size = {200, 30};
        auto element = new TextField(size, posRatios);

        // Set allowed input - a regex pattern (e.g. only DIGITS (1-9) allowed)
        element->setPattern(pattern);

        auto& rect = element->body;
        rect.setFillColor(ColorGray);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(3);

        if(textValue.empty() == false) {
            auto text = sf::Text();
            text.setFont(FontManager::Fonts["times"]);
            text.setCharacterSize(GameInterface::smallCharacterSize);

            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(1);

            text.setString(textValue);

            element->setText(text);
        }

        auto input = sf::Text();
        input.setFont(FontManager::Fonts["aleoItalic"]);
        input.setCharacterSize(GameInterface::smallCharacterSize);
        input.setFillColor(sf::Color::White);
        input.setString(fieldValue);

        // I-line, Cursour etc. <- Its the line that blinks and indicates where your next letter will be
        element->pointLine.setFont(FontManager::Fonts["times"]);
        element->pointLine.setCharacterSize(GameInterface::smallCharacterSize);
        element->pointLine.setFillColor(sf::Color::White);
        element->pointLine.setString("|");

        element->setInput(input);

        return element;
    }

    static TextField* createTextField(const std::string& fieldValue, sf::Vector2f posRatios, std::string textValue, std::wstring pattern, std::function<void()> const& onTextFieldUpdate) {
        auto element = UIElementFactory::createTextField(fieldValue, posRatios, textValue, pattern);
        element->onTextFieldUpdate = onTextFieldUpdate;

        return element;
    }

    static Button* createMenuButton(const std::string& name, const std::function<void()>& onClick, sf::Vector2f posRatios) {
        sf::Vector2f size = {200, 50};
        return createMenuButton(name, onClick, posRatios, size);
    }

    static Button* createMenuButton(const std::string& name, const std::function<void()>& onClick, sf::Vector2f posRatios, sf::Vector2f size) {
        auto element = new Button(size, posRatios, onClick);

        auto& rect = element->body;
        rect.setFillColor(ColorBlue);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(4);

        auto text = sf::Text();
        text.setFont(FontManager::Fonts["jaro"]);
        text.setCharacterSize(GameInterface::smallCharacterSize);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
        text.setString(name);
        text.setPosition(rect.getPosition());
        text.move(rect.getSize().x/2 - text.getGlobalBounds().width/2, rect.getSize().y/2 - text.getGlobalBounds().height + text.getGlobalBounds().height/3);
        element->setText(text);

        return element;
    }

    static Panel* createPanel(UIElement* parent, sf::Vector2f size, sf::Vector2f posRatios, PanelType panelType) {
        auto panel = new Panel(parent, size, posRatios);

        panel->setPanelType(panelType);

        auto& rect = panel->body;
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Green);
        rect.setOutlineThickness(3);

        return panel;
    }
};
