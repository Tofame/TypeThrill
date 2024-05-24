#pragma once
#include "Button.h"
#include "Panel.h"
#include "TextField.h"
#include "UIElement.h"
#include "../Settings.h"
#include "../Globals.h"
#include "../ResourceManagers/FontManager.h"

inline auto ColorBlue = sf::Color(52, 194, 199);
inline auto ColorGray = sf::Color(105,104,102);
inline auto ColorGrayDarkLO = sf::Color(64,64,64, 120);

class UIElementFactory {
public:
    // Button static createMenuButton(const std::string& name, const std::function<void()>& onClick);
    // std::unique_ptr<UIElement> static createButton();

    static UIElement* createTextField(const std::string& fieldValue, sf::Vector2f posRatios, std::string textValue) {
        sf::Vector2f size = {200 * Settings::getUIScale(), 30 * Settings::getUIScale()};
        sf::Vector2f position = { (float)(window.getSize().x/2 - size.x/2), (float)(window.getSize().y/2 - size.y/2) };
        auto element = new TextField(size, position);
        element->setPosRatios(posRatios.x, posRatios.y);

        auto& rect = element->body;
        rect.setFillColor(ColorGray);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(3);

        if(textValue.empty() == false) {
            auto text = sf::Text();
            text.setFont(FontManager::Fonts["times"]);
            text.setCharacterSize(GameInterface::smallCharacterSize * Settings::getUIScale());

            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
            text.setOutlineThickness(1);

            text.setString(textValue);

            element->setText(text);
        }

        auto input = sf::Text();
        input.setFont(FontManager::Fonts["aleoItalic"]);
        input.setCharacterSize(GameInterface::smallCharacterSize * Settings::getUIScale());
        input.setFillColor(sf::Color::White);
        input.setString(fieldValue);

        // I-line, Cursour etc. <- Its the line that blinks and indicates where your next letter will be
        element->pointLine.setFont(FontManager::Fonts["times"]);
        element->pointLine.setCharacterSize(GameInterface::smallCharacterSize * Settings::getUIScale());
        element->pointLine.setFillColor(sf::Color::White);
        element->pointLine.setString("|");

        element->setInput(input);

        return element;
    }

    static UIElement* createMenuButton(const std::string& name, const std::function<void()>& onClick, sf::Vector2f posRatios) {
        sf::Vector2f size = {200 * Settings::getUIScale(), 50 * Settings::getUIScale()};
        return createMenuButton(name, onClick, posRatios, size);
    }

    static UIElement* createMenuButton(const std::string& name, const std::function<void()>& onClick, sf::Vector2f posRatios, sf::Vector2f size) {
        sf::Vector2f position = { (float)(window.getSize().x/2 - size.x/2), (float)(window.getSize().y/2 - size.y/2) };
        auto menuButton = new Button(size, position, onClick);
        menuButton->setPosRatios(posRatios.x, posRatios.y);

        auto& rect = menuButton->body;
        rect.setFillColor(ColorBlue);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(4);

        auto text = sf::Text();
        text.setFont(FontManager::Fonts["jaro"]);
        text.setCharacterSize(GameInterface::smallCharacterSize * Settings::getUIScale());
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
        text.setString(name);
        text.setPosition(rect.getPosition());
        text.move(rect.getSize().x/2 - text.getGlobalBounds().width/2, rect.getSize().y/2 - text.getGlobalBounds().height + text.getGlobalBounds().height/3);
        menuButton->setText(text);

        return menuButton;
    }

    static Panel* createPanel(UIElement* parent, sf::Vector2f size, sf::Vector2f posRatios, PanelType type) {
        size.x = size.x * Settings::getUIScale();
        size.y = size.y * Settings::getUIScale();

        auto panel = new Panel(parent, size, posRatios);

        panel->setType(type);

        auto& rect = panel->body;
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Green);
        rect.setOutlineThickness(3);

        return panel;
    }
};
