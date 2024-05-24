#include "TextField.h"
#include "../Globals.h"
#include "fmt/args.h"

TextField::TextField(sf::Vector2f& size, sf::Vector2f& position) {
    this->body = sf::RectangleShape(size);
    this->body.setPosition(position);
    this->visibility = true;
    this->setPosRatios(0.1, 0.1);
}

void TextField::draw() {
    window.draw(this->body);
    window.draw(this->text);
    window.draw(this->input);
}

void TextField::handleClick() {
    if(this->getState() == FOCUSED) {
        this->setState(DEFAULT);
        return;
    }

    this->setState(FOCUSED);
}

void TextField::update() {
    sf::Vector2f parentScale = parent->body.getScale();
    this->body.setScale(parentScale);

    // Calculate position of TextField text in parent's body
    sf::FloatRect textBounds = this->text.getLocalBounds();
    auto textOffset = textBounds.top + textBounds.height/2;

    sf::Vector2f newPosition(
        parent->body.getPosition().x + this->posXRatio * parent->body.getSize().x * parentScale.x,
        parent->body.getPosition().y + this->posYRatio * parent->body.getSize().y * parentScale.y - textOffset
    );

    this->text.setPosition(newPosition);

    // Set TextField body's position
    newPosition.x = newPosition.x + textBounds.width + 16; // + random value so there is spacing
    newPosition.y = newPosition.y - (this->body.getSize().y * parentScale.y)/2 + textOffset;
    this->body.setPosition(newPosition);

    sf::FloatRect inputBounds = this->text.getLocalBounds();
    this->input.setPosition(newPosition);

    this->text.setScale(parentScale);
    this->input.setScale(parentScale);
}

void TextField::onChange() {
    if(getText().getString().getSize() <= 0) {

        return;
    }
}

bool TextField::isClicked(const sf::Vector2i& mousePos) {
    return body.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

sf::Text& TextField::getText() {
    return this->text;
}

void TextField::setText(sf::Text &text) {
    this->text = text;
}

std::string TextField::getInputString() {
    return input.getString();
}

void TextField::setInput(std::string value) {
    this->input.setString(value);
}

void TextField::setInput(sf::Text value) {
    this->input = value;
}

void TextField::move(float x, float y) {
    this->body.move(x, y);
    this->text.move(x, y);
}

void TextField::setPosition(float x, float y) {
    this->body.setPosition(x, y);
    this->text.setPosition(x, y);
}