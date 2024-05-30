#pragma once

#include <SFML/Graphics.hpp>
#include "UIElement.h"

// It's the way Label's text position is calculated in update() method.
// For example ALIGN_HALFWIDTH will do (...ratios - text's.width/2)
enum TextAlignTypes {
    ALIGN_NONE = 1,
    ALIGN_HALFWIDTH = 2,
    ALIGN_HALFHEIGHT = 3,
    ALIGN_HALFSIZE = 4
};

class TextLabel : public UIElement {
public:
    TextLabel() = default;
    TextLabel(std::string text, sf::Vector2f posRatios);

    void draw() override;
    void update() override;

    sf::Text& getText();
    void setText(sf::Text& text);

    void move(float x, float y) override;
    void setPosition(float x, float y) override;

    TextAlignTypes getAlignType();
    void setAlignType(TextAlignTypes type);
private:
    sf::Text text;
    TextAlignTypes alignType = ALIGN_HALFWIDTH;
};