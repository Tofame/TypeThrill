#pragma once

#include <SFML/Graphics.hpp>
#include "UIElement.h"
#include <regex>

class TextField : public UIElement {
public:
    TextField() = default;
    TextField(sf::Vector2f& size, sf::Vector2f& position);

    void draw() override;
    void handleClick() override;
    void update() override;
    void onWriteableKeyPressed(int mode, sf::Uint32 c) override;
    bool isClicked(const sf::Vector2i& mousePos) override;

    void updatePointLinePosition(int offsetX);

    sf::Text& getText();
    void setText(sf::Text& text);

    sf::String getInputString();
    void setInput(sf::String value);
    void setInput(sf::Text value);

    void move(float x, float y) override;
    void setPosition(float x, float y) override;

    std::regex getPattern();
    void setPattern(std::string pattern);

    sf::Text pointLine;
private:
    sf::Text text;
    sf::Text input;
    std::regex pattern;
};