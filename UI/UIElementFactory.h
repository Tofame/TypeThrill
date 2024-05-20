#pragma once
#include "Button.h"
#include "UIElement.h"
#include "../Settings.h"
#include "../Globals.h"
#include "../ResourceManagers/FontManager.h"

inline auto myBlue = sf::Color(52, 194, 199);
inline auto myWhite = sf::Color(255,255,255);

class UIElementFactory {
public:
    // Button static createMenuButton(const std::string& name, const std::function<void()>& onClick);
    // std::unique_ptr<UIElement> static createButton();

    static UIElement* createMenuButton(const std::string& name, const std::function<void()>& onClick) {
        sf::Vector2f size = {200 * Settings::getUIScale(), 50 * Settings::getUIScale()};
        sf::Vector2f position = { (float)(window.getSize().x/2 - size.x/2), (float)(window.getSize().y/2 - size.y/2) };
        auto menuButton = new Button(size, position);
        menuButton->posXRatio = 0.5;
        menuButton->posYRatio = 0.5;

        menuButton->onClick = onClick;

        auto& rect = menuButton->body;
        rect.setFillColor(myBlue);
        rect.setOutlineColor(myWhite);
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
};
