#include "Checkbox.h"
#include "../Globals.h"
#include "../Settings.h"
#include "../ResourceManagers/TextureManager.h"
#include "fmt/Core.h"

Checkbox::Checkbox(float sizeMultiplier, sf::Vector2f posRatios) {
    this->sizeMultiplier = sizeMultiplier;

    this->setPosRatios(posRatios.x, posRatios.y);

    this->checkBoxSprite.setTexture(TextureManager::Textures["checkbox"]);
    auto txtWidth = this->checkBoxSprite.getTexture()->getSize().x;
    auto txtHeight = this->checkBoxSprite.getTexture()->getSize().y;
    this->checkBoxSprite.setScale(getSizeMultiplier(), getSizeMultiplier());
    this->checkBoxSprite.setTextureRect(sf::IntRect(0, 0, txtWidth/2, txtHeight));
}

bool Checkbox::isEnabled() const {
    return this->enabled;
}

void Checkbox::disable() {
    this->enabled = false;
    auto txtWidth = this->checkBoxSprite.getTexture()->getSize().x;
    auto txtHeight = this->checkBoxSprite.getTexture()->getSize().y;
    this->checkBoxSprite.setTextureRect(sf::IntRect(0, 0, txtWidth/2, txtHeight));
}

void Checkbox::enable() {
    this->enabled = true;
    auto txtWidth = this->checkBoxSprite.getTexture()->getSize().x;
    auto txtHeight = this->checkBoxSprite.getTexture()->getSize().y;
    this->checkBoxSprite.setTextureRect(sf::IntRect(txtWidth/2, 0, txtWidth/2, txtHeight));
}

void Checkbox::draw() {
    window.draw(this->checkBoxSprite);

    if(text.getString().isEmpty() == false) {
        window.draw(this->text);
    }
}

void Checkbox::handleClick() {
    if(isEnabled()) {
        this->disable();
    } else {
        this->enable();
    }
}

void Checkbox::update() {
    sf::Vector2f parentScale = parent->body.getScale();

    auto textWidth = this->text.getLocalBounds().width * parentScale.x;

    sf::Vector2f newPosition(
        parent->body.getPosition().x + this->posXRatio * parent->body.getSize().x * parentScale.x,
        parent->body.getPosition().y + this->posYRatio * parent->body.getSize().y * parentScale.y
    );
    this->checkBoxSprite.setPosition(newPosition);
    this->checkBoxSprite.setScale(parentScale);

    if (text.getString().isEmpty() == false) {
        this->checkBoxSprite.move(textWidth + 6, 0);
        this->text.setPosition(newPosition);
        this->text.setScale(parentScale);
    }
}

bool Checkbox::isClicked(const sf::Vector2i& mousePos) {
    return checkBoxSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

float Checkbox::getSizeMultiplier() {
    return Settings::getUIScale() * sizeMultiplier;
}

sf::Text& Checkbox::getText() {
    return this->text;
}

void Checkbox::setText(sf::Text &text) {
    this->text = text;
}