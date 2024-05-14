#pragma once
#include "SFML/Graphics/Sprite.hpp"

class Checkbox {
public:
    bool isEnabled() const;
    void enable();
    void disable();

    bool isVisible() const;


    void onClick();
private:
    sf::Sprite checkBoxSprite;
    bool enabled = false;
    bool visibility = true;
};