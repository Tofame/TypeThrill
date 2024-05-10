#include "Button.h"
#include "../Globals.h"
#include <fmt/Core.h>

Button::Button() {
    //fmt::println("Default Button constructor called.");
    this->buttonRect = sf::RectangleShape({400, 400});
    this->buttonRect.setPosition({ (float)(window.getSize().x/2 - buttonRect.getSize().x/2), (float)(window.getSize().y/2 - buttonRect.getSize().y/2) });
    this->onClick = []() -> void { fmt::println("Default Button onClick"); };
}

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::function<void()>& onClick) {
    this->buttonRect = sf::RectangleShape(size);
    this->buttonRect.setPosition(position);
    this->onClick = []() -> void { fmt::println("Default Button onClick"); };
}

void Button::draw() const {
    window.draw(this->buttonRect);
}

void Button::handleClick() const {
    this->onClick();
}
