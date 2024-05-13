#include <fmt/Core.h>

#include "Button.h"
#include "../Globals.h"
#include "../Settings.h"



Button::Button() {
    //fmt::println("Default Button constructor called.");
    this->buttonRect = sf::RectangleShape({200 * Settings::getUIScale(), 50 * Settings::getUIScale()});
    this->buttonRect.setPosition({ (float)(window.getSize().x/2 - buttonRect.getSize().x/2), (float)(window.getSize().y/2 - buttonRect.getSize().y/2) });
    this->visibility = true;
    this->onClick = []() -> void { fmt::println("Default Button onClick"); };
}

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::function<void()>& onClick) {
    this->buttonRect = sf::RectangleShape(size);
    this->buttonRect.setPosition(position);
    this->visibility = true;
    this->onClick = []() -> void { fmt::println("Default Button onClick"); };
}

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position) {
    this->buttonRect = sf::RectangleShape(size);
    this->buttonRect.setPosition(position);
    this->visibility = true;
}

void Button::draw() const {
    window.draw(this->buttonRect);
    window.draw(this->text);
}

void Button::handleClick() const {
    this->onClick();
}

bool Button::isClicked(const sf::Vector2i& mousePos) const {
    return buttonRect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

sf::Text& Button::getText() {
    return this->text;
}

void Button::setText(sf::Text &text) {
    this->text = text;
}

void Button::move(float x, float y) {
    this->buttonRect.move(x, y);
    this->text.move(x, y);
}
void Button::setPosition(float x, float y) {
    this->buttonRect.setPosition(x, y);
    this->text.setPosition(x, y);
}


void Button::setVisible(bool value) { this->visibility = value; };
bool Button::isVisible() const { return visibility; };