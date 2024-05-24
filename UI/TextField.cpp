#include "TextField.h"
#include "../Globals.h"
#include "fmt/Core.h"

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

    if(getState() == FOCUSED) {
        auto currentColor = this->pointLine.getFillColor();
        auto alpha = currentColor.a;
        if(alpha <= 0) {
            currentColor.a = 255;
            this->pointLine.setFillColor(currentColor);
        } else {
            currentColor.a -= 10;
            this->pointLine.setFillColor(currentColor);
        }

        window.draw(this->pointLine);
    }
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

    auto textWidth = this->text.getLocalBounds().width * parentScale.x;

    sf::Vector2f newPosition(
        parent->body.getPosition().x + this->posXRatio * parent->body.getSize().x * parentScale.x,
        parent->body.getPosition().y + this->posYRatio * parent->body.getSize().y * parentScale.y
    );
    this->body.setPosition(newPosition);
    this->body.move(textWidth + 6, 0);
    this->body.setScale(parentScale);

    this->text.setPosition(newPosition);
    this->text.setScale(parentScale);

    auto bodyPosition = this->body.getPosition();
    this->input.setPosition(bodyPosition);
    this->input.setScale(parentScale);

    this->updatePointLinePosition(0);
}

void TextField::updatePointLinePosition(int offsetX) {
    this->pointLine.setPosition(this->input.getPosition());
    this->pointLine.move(this->input.getGlobalBounds().width + offsetX, -4);

    this->pointLine.setScale(this->parent->body.getScale());
}

void TextField::onWriteableKeyPressed(int mode, sf::Uint32 c) {
    auto inputString = getInputString();

    if(mode == 0) {
        if(inputString.isEmpty()) {
            return;
        }

        inputString.erase(inputString.getSize() - 1);
    } else {
        inputString += c;
    }

    this->input.setString(inputString);
    this->updatePointLinePosition(0);
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

sf::String TextField::getInputString() {
    return input.getString();
}

void TextField::setInput(sf::String value) {
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