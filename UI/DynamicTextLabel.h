#pragma once

#include <functional>
#include <SFML/Graphics.hpp>
#include "TextLabel.h"

class DynamicTextLabel : public TextLabel {
public:
    DynamicTextLabel() = default;
    DynamicTextLabel(sf::Vector2f ratios, std::function<std::string()> const &onUpdateString);

    void draw() override;
    void update() override;

    void move(float x, float y) override;
    void setPosition(float x, float y) override;

    sf::Text& getText();
    void setText(sf::Text& text);

    // Source of knowledge: https://stackoverflow.com/questions/20353210/what-is-the-purpose-of-stdfunction-and-how-do-i-use-it
    // void <= doesn't return anything, () <= empty brackets, takes no parameters. We can now save such lambdas into TextField now.
    std::function<std::string()> onUpdateString = []() -> std::string { return ""; };

private:
    sf::Text text;
};