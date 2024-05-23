#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

enum UIElementState {
    DEFAULT = 0,
    HOVERED = 1,
    CLICKED = 2
};

class UIElement {
public:
    virtual ~UIElement() = default;

    virtual void draw() {};
    virtual void update() {};
    virtual void handleClick() {};
    virtual void updateState() {};

    virtual bool isClicked(const sf::Vector2i& mousePos) { return false; };

    UIElement* parent = nullptr;

    void setParent(UIElement* parent) { this->parent = parent; };
    UIElement* getParent() { return parent; };

    float posXRatio = 0;
    float posYRatio = 0;
    void setPosRatios(float x, float y) { posXRatio = x; posYRatio = y; };

    sf::RectangleShape body;

    bool isVisible() { return visibility; };
    void setVisibility(bool value) { visibility = value; };
    void toggleVisibility(bool value) { visibility = !visibility; };

    bool visibility = true;

    void setState(UIElementState state) { this->state = state; };
    UIElementState getState() { return state; };
protected:
    UIElementState state = DEFAULT;
};