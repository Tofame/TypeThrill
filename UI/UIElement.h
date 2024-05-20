#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class UIElement {
public:
    virtual ~UIElement() = default;

    virtual void draw() {};
    virtual void update() {};
    virtual void handleClick() {};

    virtual bool isClicked(const sf::Vector2i& mousePos) { return false; };

    UIElement* parent = nullptr;

    void setParent(UIElement* parent) { this->parent = parent; };
    UIElement* getParent() { return parent; };

    float posXRatio = 0;
    float posYRatio = 0;

    sf::RectangleShape body;
};