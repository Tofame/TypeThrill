#include "UIButton.h"
#include "../Globals.h"
#include <fmt/Core.h>

UIButton::UIButton() {
    //fmt::println("Default UIButton constructor called.");
    this->buttonRect = sf::RectangleShape({400, 400});
    this->buttonRect.setPosition({ (float)(window.getSize().x/2 - buttonRect.getSize().x/2), (float)(window.getSize().y/2 - buttonRect.getSize().y/2) });
    this->onClick = []() -> void { fmt::println("Default UIButton"); };
}

UIButton::UIButton(const sf::Vector2f& size, const sf::Vector2f& position, const std::function<void()>& onClick) {
    this->buttonRect = sf::RectangleShape(size);
    this->buttonRect.setPosition(position);
    this->onClick = []() -> void { fmt::println("Default UIButton"); };
}

void UIButton::handleClick() const {
    this->onClick();
}