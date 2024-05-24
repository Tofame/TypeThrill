#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

enum UIElementState {
    DEFAULT = 0,
    HOVERED = 1,
    FOCUSED = 2
};

class UIElement {
public:
    virtual ~UIElement() = default;

    virtual void draw() {};
    virtual void update() {};
    virtual void handleClick() {};
    virtual void onWriteableKeyPressed(int mode, sf::Uint32 c) {}; // Writeable means backspace/space/delete and alphabet

    virtual bool isClicked(const sf::Vector2i& mousePos) { return false; };

    virtual void move(float x, float y) {};
    virtual void setPosition(float x, float y) {};

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

    void setState(UIElementState state) {
        this->state = state;

        switch(state) {
            case DEFAULT:
                this->body.setOutlineColor(sf::Color::White);
                break;
            case HOVERED:
                this->body.setOutlineColor(sf::Color::Yellow);
                break;
            case FOCUSED:
                this->body.setOutlineColor(sf::Color::Cyan);
                break;
            default:
                this->body.setOutlineColor(sf::Color::White);
        }
    };
    UIElementState getState() { return state; };
protected:
    UIElementState state = DEFAULT;
};