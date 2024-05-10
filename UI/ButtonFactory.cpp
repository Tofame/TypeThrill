#include <fmt/Core.h>

#include "../Globals.h"
#include "ButtonFactory.h"
#include "../Game/GameInterface.h"

std::map<std::string, std::unique_ptr<Button>> ButtonFactory::Buttons;

void ButtonFactory::setupButtons() {
    ButtonFactory::Buttons["Default1"] = ButtonFactory::createUIButton();
}

std::unique_ptr<Button> ButtonFactory::createUIButton() {
    const sf::Vector2f size = { 300.0, 100.0 };
    const sf::Vector2f position = { (float)(window.getSize().x/2 - size.x/2), (float)(window.getSize().y/2 - size.y/2) };
    auto clickLambda = []() -> void {
        fmt::println("Default UIButton Click");
    };

    return std::make_unique<UIButton>(size, position, clickLambda);
}