#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

#include "Button.h"

class Button {
public:
    Button();
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::function<void()>& onClick);

    sf::RectangleShape buttonRect;
    // Source of knowledge: https://stackoverflow.com/questions/20353210/what-is-the-purpose-of-stdfunction-and-how-do-i-use-it
    // void <= doesn't return anything, () <= empty brackets, takes no parameters. We can now save such lambdas into onClick now.
    std::function<void()> onClick;

    void draw() const;
    void handleClick() const;
    bool isClicked(const sf::Vector2i& mousePos) const;
};