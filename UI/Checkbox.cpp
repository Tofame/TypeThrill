#include "Checkbox.h"
#include "../Globals.h"

auto rectDisabled = sf::IntRect(0, 0, 32, 32);
auto rectEnabled = sf::IntRect(32, 0, 32, 32);

bool Checkbox::isEnabled() const {
    return this->enabled;
}

void Checkbox::disable() {
    this->enabled = false;
    this->checkBoxSprite.setTextureRect(rectDisabled);
}

void Checkbox::enable() {
    this->enabled = true;
    this->checkBoxSprite.setTextureRect(rectEnabled);
}

bool Checkbox::isVisible() const {
    return this->visibility;
}

void Checkbox::draw() {
    window.draw(this->checkBoxSprite);
}

void Checkbox::handleClick() {
    if(isEnabled()) {
        this->disable();
    } else {
        this->disable();
    }
}

void Checkbox::update() {
    // Scale and position when window resize
}

bool Checkbox::isClicked(const sf::Vector2i& mousePos) {
    return checkBoxSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

void onClick();
