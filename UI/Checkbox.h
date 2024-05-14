#pragma once
#include "UIElement.h"
#include "SFML/Graphics/Sprite.hpp"

class Checkbox : public UIElement {
public:
    bool isEnabled() const;
    void enable();
    void disable();

    bool isVisible() const;

    void draw() override;
    void handleClick() override;
    void update() override;

    bool isClicked(const sf::Vector2i& mousePos) override;
private:
    sf::Sprite checkBoxSprite;
    bool enabled = false;
    bool visibility = true;
};