#include "ButtonFactory.h"
#include "../Globals.h"
#include "../Settings.h"

#include <fmt/Core.h>

std::map<std::string, Button> ButtonFactory::Buttons;

void ButtonFactory::setupButtons() {
    ButtonFactory::Buttons["Default1"] = ButtonFactory::createButton();
}

Button ButtonFactory::createButton() {
    const sf::Vector2f size = {200 * Settings::getUIScale(), 50 * Settings::getUIScale()};
    const sf::Vector2f position = { (float)(window.getSize().x/2 - size.x/2), (float)(window.getSize().y/2 - size.y/2) };
    auto clickLambda = []() -> void {
        fmt::println("Default Button Click");
    };

    return Button(size, position, clickLambda);
}