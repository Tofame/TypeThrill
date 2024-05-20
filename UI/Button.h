#pragma once

#include <functional>
#include <SFML/Graphics.hpp>
#include "UIElement.h"

class Button : public UIElement {
public:
    Button();
    Button(sf::Vector2f& size, sf::Vector2f& position, const std::function<void()>& onClick);
    Button(sf::Vector2f& size, sf::Vector2f& position);

    // Source of knowledge: https://stackoverflow.com/questions/20353210/what-is-the-purpose-of-stdfunction-and-how-do-i-use-it
    // void <= doesn't return anything, () <= empty brackets, takes no parameters. We can now save such lambdas into onClick now.
    std::function<void()> onClick;

    void draw() override;
    void handleClick() override;
    void update() override;
    bool isClicked(const sf::Vector2i& mousePos) override;

    sf::Text& getText();
    void setText(sf::Text& text);

    void move(float x, float y);
    void setPosition(float x, float y);
private:
    sf::Text text;
};