#include <fmt/Core.h>

#include "Button.h"
#include "../Globals.h"
#include "../Settings.h"

Button::Button() {
    //fmt::println("Default Button constructor called.");
    this->buttonRect = sf::RectangleShape({200 * Settings::getUIScale(), 50 * Settings::getUIScale()});
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

bool Button::isClicked(const sf::Vector2i& mousePos) const {
    return buttonRect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}