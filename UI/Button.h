#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

#include "Button.h"

class Button {
public:
    Button();
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::function<void()>& onClick);
    Button(const sf::Vector2f& size, const sf::Vector2f& position);

    sf::RectangleShape buttonRect;
    // Source of knowledge: https://stackoverflow.com/questions/20353210/what-is-the-purpose-of-stdfunction-and-how-do-i-use-it
    // void <= doesn't return anything, () <= empty brackets, takes no parameters. We can now save such lambdas into onClick now.
    std::function<void()> onClick;

    bool isVisible() const;
    void setVisible(bool value);

    void draw() const;
    void handleClick() const;
    bool isClicked(const sf::Vector2i& mousePos) const;

    sf::Text& getText();
    void setText(sf::Text& text);

    void move(float x, float y);
    void setPosition(float x, float y);
private:
    bool visibility;
    sf::Text text;
};