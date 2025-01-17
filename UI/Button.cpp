#include <fmt/Core.h>

#include "Button.h"
#include "../Globals.h"
#include "../Settings.h"

auto defaultButtonLambda = []() -> void { fmt::println("Default Button onClick"); };

Button::Button(sf::Vector2f& size, sf::Vector2f& posRatios, const std::function<void()>& onClick) {
    this->setType(BUTTON);

    this->body = sf::RectangleShape(size);
    this->posRatio.setValues(posRatios);

    this->onClick = onClick;

    this->setVisibility(true);
}

Button::Button(sf::Vector2f& size, sf::Vector2f& posRatios) : Button::Button(size, posRatios, defaultButtonLambda) {}

void Button::draw() {
    window.draw(this->body);
    window.draw(this->text);
}

void Button::handleClick() {
    if(isEnabled() == false)
        return;

    this->onClick();

    // If onClick made the button invisible then we remove the "Hovered" state.
    // Example where without this code things would look weird:
    // In main menu click 'Settings', then in Settings click 'Back'. DON`T move mouse. Then the 'Settings' button would be hovered.
    if(this->isVisible() == false || this->getParent() && this->getParent()->isVisible() == false) {
        this->setState(DEFAULT);
    }
}

void Button::update() {
    sf::Vector2f parentScale = parent->body.getScale();
    this->body.setScale(parentScale);

    sf::Vector2f newPosition(
        parent->body.getPosition().x + (this->posRatio.getX() * parent->body.getSize().x * parentScale.x) - (this->body.getSize().x * parentScale.x)/2,
        parent->body.getPosition().y + (this->posRatio.getY() * parent->body.getSize().y * parentScale.y) - (this->body.getSize().y * parentScale.y)/2
    );

    this->body.setPosition(newPosition);

    sf::FloatRect textBounds = this->text.getLocalBounds();
    this->text.setOrigin(textBounds.left + textBounds.width/2, textBounds.top + textBounds.height/2);

    sf::Vector2f buttonCenter(
        newPosition.x + (this->body.getSize().x * parentScale.x)/2,
        newPosition.y + (this->body.getSize().y * parentScale.y)/2
    );

    this->text.setScale(parentScale);
    this->text.setPosition(buttonCenter);
}

bool Button::isMouseOver(const sf::Vector2i& mousePos) {
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