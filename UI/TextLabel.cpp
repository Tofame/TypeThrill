#include "TextLabel.h"

#include "../Globals.h"
#include "../Game/GameInterface.h"
#include "../ResourceManagers/FontManager.h"

TextLabel::TextLabel(std::string text, sf::Vector2f posRatios) {
    this->setType(TEXTLABEL);

    this->posRatio.setValues(posRatios);

    auto sfText = sf::Text();
    sfText.setFont(FontManager::Fonts["jaro"]);
    sfText.setCharacterSize(GameInterface::smallCharacterSize);
    sfText.setFillColor(sf::Color::White);
    sfText.setOutlineColor(sf::Color::Black);
    sfText.setOutlineThickness(1);
    sfText.setString(text);

    this->setText(sfText);

    this->visibility = true;
}

void TextLabel::draw() {
    if(text.getString().isEmpty() == false) {
        window.draw(this->text);
    }
}

void TextLabel::update() {
    sf::Vector2f parentScale = parent->body.getScale();

    this->text.setScale(parentScale);

    auto textBounds = this->text.getGlobalBounds();

    sf::Vector2f newPosition(
        parent->body.getPosition().x + this->posRatio.getX() * parent->body.getSize().x * parentScale.x - textBounds.width/2,
        parent->body.getPosition().y + this->posRatio.getY() * parent->body.getSize().y * parentScale.y
    );
    this->text.setPosition(newPosition);
}

sf::Text& TextLabel::getText() {
    return this->text;
}

void TextLabel::setText(sf::Text &text) {
    this->text = text;
}

void TextLabel::move(float x, float y) {
    this->text.move(x, y);
}

void TextLabel::setPosition(float x, float y) {
    this->text.setPosition(x, y);
}