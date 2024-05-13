#include "Checkbox.h"

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

void onClick();