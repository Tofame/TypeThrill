#pragma once

#include "UIElement.h"
#include "SFML/Graphics/Text.hpp"

class Word : public UIElement {
public:
    Word();

    void draw() override;
    void update() override;

    sf::Text& getText();
    void setText(sf::Text& text);

    void move(float x, float y) override;
    void setPosition(float x, float y) override;
private:
    sf::Text text;
};
