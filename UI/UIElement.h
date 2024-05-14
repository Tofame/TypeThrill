#pragma once
#include "SFML/System/Vector2.hpp"

class UIElement {
public:
    virtual ~UIElement() = default;

    virtual void draw() {};
    virtual void update() {};
    virtual void handleClick() {};

    virtual bool isClicked(const sf::Vector2i& mousePos) { return false; };
};
