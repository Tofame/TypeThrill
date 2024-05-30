#include "DynamicTextLabel.h"

#include "../Globals.h"
#include "../Game/GameInterface.h"
#include "../ResourceManagers/FontManager.h"

DynamicTextLabel::DynamicTextLabel(sf::Vector2f posRatios, std::function<std::string()> const& onUpdateString) {
    this->setType(DYNAMICTEXTLABEL);

    this->posRatio.setValues(posRatios);

    auto sfText = sf::Text();
    sfText.setFont(FontManager::Fonts["jaro"]);
    sfText.setCharacterSize(GameInterface::smallCharacterSize);
    sfText.setFillColor(sf::Color::White);
    sfText.setOutlineColor(sf::Color::Black);
    sfText.setOutlineThickness(1);

    this->onUpdateString = onUpdateString;
    sfText.setString(this->onUpdateString());

    this->setText(sfText);

    this->setVisibility(true);
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
