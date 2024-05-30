#include "Checkbox.h"
#include "../Globals.h"
#include "../Settings.h"
#include "../ResourceManagers/TextureManager.h"
#include "fmt/Core.h"

Checkbox::Checkbox(float sizeMultiplier, sf::Vector2f posRatios, bool defaultChecked) {
    this->setType(CHECKBOX);

    this->sizeMultiplier = sizeMultiplier;

    this->posRatio.setValues(posRatios);

    this->texture = TextureManager::loadResource("UI/checkbox.png");
    this->checkBoxSprite.setTexture(this->texture);

    auto txtWidth = this->checkBoxSprite.getTexture()->getSize().x/2;
    auto txtHeight = this->checkBoxSprite.getTexture()->getSize().y/2;
    this->checkBoxSprite.setTextureRect(sf::IntRect(0, 0, txtWidth, txtHeight));

    this->checkBoxSprite.setScale(getSizeMultiplier(), getSizeMultiplier());

    this->state = DEFAULT;
    if(defaultChecked) {
        this->check();
    } else {
        this->uncheck();
    }
}

bool Checkbox::isChecked() const {
    return this->checked == true;
}

void Checkbox::uncheck() {
    this->checked = false;
    auto txtWidth = this->checkBoxSprite.getTexture()->getSize().x/2;
    auto txtHeight = this->checkBoxSprite.getTexture()->getSize().y/2;
    auto checkboxStateOffset = this->getState() == HOVERED ? txtHeight : 0;

    this->checkBoxSprite.setTextureRect(sf::IntRect(0, checkboxStateOffset, txtWidth, txtHeight));
}

void Checkbox::check() {
    this->checked = true;
    auto txtWidth = this->checkBoxSprite.getTexture()->getSize().x/2;
    auto txtHeight = this->checkBoxSprite.getTexture()->getSize().y/2;
    auto checkboxStateOffset = this->getState() == HOVERED ? txtHeight : 0;

    this->checkBoxSprite.setTextureRect(sf::IntRect(txtWidth, checkboxStateOffset, txtWidth, txtHeight));
}

void Checkbox::draw() {
    window.draw(this->checkBoxSprite);

    if(text.getString().isEmpty() == false) {
        window.draw(this->text);
    }
}

void Checkbox::handleClick() {
    if(isEnabled() == false)
        return;

    if(isChecked()) {
        this->uncheck();
    } else {
        this->check();
    }

    this->onCheckboxUpdate(isChecked());
}

void Checkbox::update() {
    sf::Vector2f parentScale = parent->body.getScale() * getSizeMultiplier();

    auto textWidth = this->text.getLocalBounds().width * parentScale.x;

    sf::Vector2f newPosition(
        parent->body.getPosition().x + this->posRatio.getX() * parent->body.getSize().x * parentScale.x,
        parent->body.getPosition().y + this->posRatio.getY() * parent->body.getSize().y * parentScale.y
    );
    this->checkBoxSprite.setPosition(newPosition);
    this->checkBoxSprite.setScale(parentScale);

    if (text.getString().isEmpty() == false) {
        this->checkBoxSprite.move(UIElement::offsetBodyAfterText * parentScale.x, 0);
        this->text.setPosition(newPosition);
        this->text.setScale(parentScale);
    }
}

void Checkbox::setState(UIElementState state) {
    if(this->getState() == state) {
        return;
    }

    this->state = state;

    auto singleFrameWidth = this->checkBoxSprite.getTexture()->getSize().x/2; // 2x2 texture
    auto singleFrameHeight = this->checkBoxSprite.getTexture()->getSize().y/2; // 2x2 texture

    switch(state) {
        case DEFAULT:
            this->checkBoxSprite.setTextureRect(sf::IntRect((this->isChecked() ? singleFrameWidth : 0), 0, singleFrameWidth, singleFrameHeight));
            break;
        case HOVERED:
            this->checkBoxSprite.setTextureRect(sf::IntRect((this->isChecked() ? singleFrameWidth : 0), singleFrameHeight, singleFrameWidth, singleFrameHeight));
            break;
        case FOCUSED:
            break;
        default:
            this->checkBoxSprite.setTextureRect(sf::IntRect((this->isChecked() ? singleFrameWidth : 0), 0, singleFrameWidth, singleFrameHeight));
    }
}

bool Checkbox::isMouseOver(const sf::Vector2i& mousePos) {
    return checkBoxSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

float Checkbox::getSizeMultiplier() {
    return sizeMultiplier;
}

sf::Text& Checkbox::getText() {
    return this->text;
}

void Checkbox::setText(sf::Text &text) {
    this->text = text;
}