#include "DynamicTextLabel.h"

#include "../Globals.h"
#include "../Game/GameInterface.h"
#include "../ResourceManagers/FontManager.h"

DynamicTextLabel::DynamicTextLabel(sf::Vector2f ratios, std::function<std::string()> const& onUpdateString) {
    this->setType(DYNAMICTEXTLABEL);

    auto sfText = sf::Text();
    sfText.setFont(FontManager::Fonts["jaro"]);
    sfText.setCharacterSize(GameInterface::smallCharacterSize);
    sfText.setFillColor(sf::Color::White);
    sfText.setOutlineColor(sf::Color::Black);
    sfText.setOutlineThickness(1);

    this->onUpdateString = onUpdateString;
    sfText.setString(this->onUpdateString());

    this->setText(sfText);

    this->visibility = true;
    this->setPosRatios(ratios.x, ratios.y);
}

void DynamicTextLabel::draw() {
    if(text.getString().isEmpty() == false) {
        window.draw(this->text);
    }
}

void DynamicTextLabel::update() {
    sf::Vector2f parentScale = parent->body.getScale();

    std::string stringValue = this->onUpdateString();
    this->getText().setString(stringValue);

    this->text.setScale(parentScale);

    auto textBounds = this->text.getGlobalBounds();

    sf::Vector2f newPosition(
        parent->body.getPosition().x + this->posXRatio * parent->body.getSize().x * parentScale.x,
        parent->body.getPosition().y + this->posYRatio * parent->body.getSize().y * parentScale.y
    );
    this->text.setPosition(newPosition);
}

void DynamicTextLabel::move(float x, float y) {
    this->text.move(x, y);
}

void DynamicTextLabel::setPosition(float x, float y) {
    this->text.setPosition(x, y);
}

sf::Text& DynamicTextLabel::getText() {
    return this->text;
}

void DynamicTextLabel::setText(sf::Text& text) {
    this->text = text;
}
