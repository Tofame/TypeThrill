#include "ButtonFactory.h"
#include "../Globals.h"
#include "../Settings.h"

#include <fmt/Core.h>

std::map<std::string, std::vector<Button>> ButtonFactory::Buttons;

// I just don't want to construct the color every time in my create methods
auto myBlue = sf::Color(52, 194, 199);
auto myBlack = sf::Color(22, 22, 23);

void ButtonFactory::setupButtons() {
    auto menuButtons = std::vector<Button>();
    menuButtons.push_back(ButtonFactory::createMenuButton("Play", []() -> void { fmt::println("Game Start!"); }));
    menuButtons.push_back(ButtonFactory::createMenuButton("Load", []() -> void { fmt::println("Game Load!"); }));
    menuButtons.push_back(ButtonFactory::createMenuButton("Exit", []() -> void { fmt::println("Game Exit!"); }));
    int moveCount = 1;
    for(auto& B : menuButtons) {
        B.buttonRect.move(0, moveCount * (15 + B.buttonRect.getSize().y));
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

Button ButtonFactory::createMenuButton(const std::string& text, const std::function<void()>& onClick) {
    const sf::Vector2f size = {200 * Settings::getUIScale(), 50 * Settings::getUIScale()};
    const sf::Vector2f position = { (float)(window.getSize().x/2 - size.x/2), (float)(window.getSize().y/2 - size.y/2) };
    auto MenuButton = Button(size, position, onClick);

    MenuButton.onClick = onClick;

    MenuButton.buttonRect.setFillColor(myBlue);
    MenuButton.buttonRect.setOutlineColor(myBlack);
    MenuButton.buttonRect.setOutlineThickness(2);

    return MenuButton;
}
