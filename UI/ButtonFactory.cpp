#include "ButtonFactory.h"
#include "../Globals.h"
#include "../Settings.h"
#include "../Game/GameInterface.h"
#include "../Game/Game.h"

#include <fmt/Core.h>

#include "../ResourceManagers/FontManager.h"

std::map<std::string, std::vector<Button>> ButtonFactory::Buttons;

// I just don't want to construct the color every time in my create methods
auto myBlue = sf::Color(52, 194, 199);
auto myWhite = sf::Color(255,255,255);

void ButtonFactory::setupButtons() {
    auto menuButtons = std::vector<Button>();
    menuButtons.push_back(ButtonFactory::createMenuButton("Play", []() -> void { Game::setGameState(Game::STATE_PLAYING); }));
    menuButtons.push_back(ButtonFactory::createMenuButton("Load", []() -> void { GameInterface::setMenuState(GameInterface::MENU_LOAD); }));
    menuButtons.push_back(ButtonFactory::createMenuButton("Settings", []() -> void { GameInterface::setMenuState(GameInterface::MENU_SETTINGS); }));
    menuButtons.push_back(ButtonFactory::createMenuButton("Exit", []() -> void { exit(0); }));
    int moveCount = 1;
    for(auto& B : menuButtons) {
        B.buttonRect.move(0, moveCount * (15 + B.buttonRect.getSize().y));
        B.getText().move(0, moveCount * (15 + B.buttonRect.getSize().y));
        moveCount++;
    }
    ButtonFactory::Buttons["MenuDefault"] = menuButtons;
}

Button ButtonFactory::createButton() {
    const sf::Vector2f size = {200 * Settings::getUIScale(), 50 * Settings::getUIScale()};
    const sf::Vector2f position = { (float)(window.getSize().x/2 - size.x/2), (float)(window.getSize().y/2 - size.y/2) };
    auto clickLambda = []() -> void {
        fmt::println("Default Button Click");
    };

    return Button(size, position, clickLambda);
}

Button ButtonFactory::createMenuButton(const std::string& name, const std::function<void()>& onClick) {
    const sf::Vector2f size = {200 * Settings::getUIScale(), 50 * Settings::getUIScale()};
    const sf::Vector2f position = { (float)(window.getSize().x/2 - size.x/2), (float)(window.getSize().y/2 - size.y/2) };
    auto MenuButton = Button(size, position);

    MenuButton.onClick = onClick;

    auto& rect = MenuButton.buttonRect;
    rect.setFillColor(myBlue);
    rect.setOutlineColor(myWhite);
    rect.setOutlineThickness(4);

    auto text = sf::Text();
    text.setFont(FontManager::Fonts["jaro"]);
    text.setCharacterSize(24 * Settings::getUIScale());
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1);
    text.setString(name);
    text.setPosition(rect.getPosition());
    text.move(rect.getSize().x/2 - text.getGlobalBounds().width/2, rect.getSize().y/2 - text.getGlobalBounds().height + text.getGlobalBounds().height/3);
    MenuButton.setText(text);

    return MenuButton;
}
