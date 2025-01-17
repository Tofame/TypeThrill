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

    this->setVisibility(true);
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

    float alignmentValueX = 0;
    float alignmentValueY = 0;
    switch(getAlignType()) {
        case ALIGN_HALFWIDTH:
            alignmentValueX -= textBounds.width/2;
            break;
        case ALIGN_HALFHEIGHT:
            alignmentValueY -= textBounds.height/2;
            break;
        case ALIGN_HALFSIZE:
            alignmentValueX -= textBounds.width/2;
            alignmentValueY -= textBounds.height/2;
            break;
        case ALIGN_NONE:
        default:
            break;
    }

    sf::Vector2f newPosition(
        parent->body.getPosition().x + this->posRatio.getX() * parent->body.getSize().x * parentScale.x + alignmentValueX,
        parent->body.getPosition().y + this->posRatio.getY() * parent->body.getSize().y * parentScale.y + alignmentValueY
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

TextAlignTypes TextLabel::getAlignType() {
    return this->alignType;
}

void TextLabel::setAlignType(TextAlignTypes type) {
    this->alignType = type;
}
