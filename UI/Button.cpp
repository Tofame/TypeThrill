#include <fmt/Core.h>

#include "Button.h"
#include "../Globals.h"
#include "../Settings.h"

auto defaultButtonLambda = []() -> void { fmt::println("Default Button onClick"); };

Button::Button(sf::Vector2f& size, sf::Vector2f& position, const std::function<void()>& onClick) {
    this->body = sf::RectangleShape(size);
    this->body.setPosition(position);
    this->visibility = true;
    this->onClick = onClick;
}

Button::Button(sf::Vector2f& size, sf::Vector2f& position) : Button::Button(size, position, defaultButtonLambda) {};

void Button::draw() {
    window.draw(this->body);
    window.draw(this->text);
}

void Button::handleClick() {
    this->onClick();
}

void Button::update() {
    sf::Vector2f parentScale = parent->body.getScale();
    this->body.setScale(parentScale);

    sf::Vector2f newPosition(
        parent->body.getPosition().x + (this->posXRatio * parent->body.getSize().x * parentScale.x) - (this->body.getSize().x * parentScale.x) / 2.0f,
        parent->body.getPosition().y + (this->posYRatio * parent->body.getSize().y * parentScale.y) - (this->body.getSize().y * parentScale.y) / 2.0f
    );

    this->body.setPosition(newPosition);

    sf::FloatRect textBounds = this->text.getLocalBounds();
    this->text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);

    sf::Vector2f buttonCenter(
        newPosition.x + (this->body.getSize().x * parentScale.x) / 2.0f,
        newPosition.y + (this->body.getSize().y * parentScale.y) / 2.0f
    );

    this->text.setScale(parentScale);
    this->text.setPosition(buttonCenter);
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