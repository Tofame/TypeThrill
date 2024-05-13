#pragma once
#include "SFML/Graphics/Sprite.hpp"

class Checkbox {
public:
    bool Checkbox::isEnabled() const;
    void Checkbox::enable();
    void Checkbox::disable();

    bool Checkbox::isVisible() const;


    void Checkbox::onClick();
private:
    sf::Sprite checkBoxSprite;
    bool enabled = false;
    bool visibility = true;
};