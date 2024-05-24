#pragma once

#include <SFML/Graphics.hpp>
#include "UIElement.h"

class TextField : public UIElement {
public:
    TextField() = default;
    TextField(sf::Vector2f& size, sf::Vector2f& position);

    void draw() override;
    void handleClick() override;
    void update() override;
    void onWriteableKeyPressed(int mode, char c) override;
    bool isClicked(const sf::Vector2i& mousePos) override;

    sf::Text& getText();
    void setText(sf::Text& text);

    std::string getInputString();
    void setInput(std::string value);
    void setInput(sf::Text value);

    void move(float x, float y);
    void setPosition(float x, float y);
private:
    sf::Text text;
    sf::Text input;
};