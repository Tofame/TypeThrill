#pragma once

#include <SFML/Graphics.hpp>
#include "UIElement.h"

class TextLabel : public UIElement {
public:
    TextLabel() = default;
    TextLabel(std::string text, sf::Vector2f ratios);

    void draw() override;
    void update() override;

    sf::Text& getText();
    void setText(sf::Text& text);

    void move(float x, float y) override;
    void setPosition(float x, float y) override;
private:
    sf::Text text;
};