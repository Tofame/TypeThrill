#include <fmt/Core.h>

#include "Button.h"
#include "../Globals.h"
#include "../Settings.h"

Button::Button() {
    //fmt::println("Default Button constructor called.");
    this->body = sf::RectangleShape({200 * Settings::getUIScale(), 50 * Settings::getUIScale()});
    this->body.setPosition({ (float)(window.getSize().x/2 - body.getSize().x/2), (float)(window.getSize().y/2 - body.getSize().y/2) });
    this->visibility = true;
    this->onClick = []() -> void { fmt::println("Default Button onClick"); };
}

Button::Button(sf::Vector2f& size, sf::Vector2f& position, const std::function<void()>& onClick) {
    this->body = sf::RectangleShape(size);
    this->body.setPosition(position);
    this->visibility = true;
    this->onClick = []() -> void { fmt::println("Default Button onClick"); };
}

Button::Button(sf::Vector2f& size, sf::Vector2f& position) {
    this->body = sf::RectangleShape(size);
    this->body.setPosition(position);
    this->visibility = true;
}

void Button::draw() {
    window.draw(this->body);
    window.draw(this->text);
}

void Button::handleClick() {
    this->onClick();
}

void Button::update() {

}

bool Button::isClicked(const sf::Vector2i& mousePos) {
    return body.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

sf::Text& Button::getText() {
    return this->text;
}

void Button::setText(sf::Text &text) {
    this->text = text;
}

void Button::move(float x, float y) {
    this->body.move(x, y);
    this->text.move(x, y);
}
void Button::setPosition(float x, float y) {
    this->body.setPosition(x, y);
    this->text.setPosition(x, y);
}


void Button::setVisible(bool value) { this->visibility = value; };
bool Button::isVisible() const { return visibility; };