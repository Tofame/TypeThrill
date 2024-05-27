#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

enum UIElementState {
    DEFAULT = 0,
    HOVERED = 1,
    FOCUSED = 2,
    FOCUSED_ALWAYS = 3
};

enum UIElementType {
    UIELEMENT = 0,
    BUTTON = 1,
    CHECKBOX = 2,
    COMBOBOX = 3,
    DYNAMICTEXTLABEL = 4,
    TEXTLABEL = 5,
    PANEL = 6,
    TEXTFIELD = 7,
    WORD = 8,
};

class UIElement {
public:
    inline static float offsetBodyAfterText = 250; // Defines the offset in case some element has both text and some field
    // Example would be TextField which has text and input field, and this will define the distance beetween them

    virtual ~UIElement() = default;

    virtual void draw() {};
    virtual void update() {};
    virtual void handleClick() {};
    virtual void onWriteableKeyPressed(int mode, sf::Uint32 c) {}; // Writeable means backspace/space/delete and alphabet

    virtual bool isMouseOver(const sf::Vector2i& mousePos) { return false; };

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

    virtual void setState(UIElementState state) {
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
            case FOCUSED_ALWAYS:
                this->body.setOutlineColor(sf::Color(196, 113, 18)); // Orange
                break;
            default:
                this->body.setOutlineColor(sf::Color::White);
        }
    };
    UIElementState getState() { return state; };


    void setType(UIElementType type) {
        this->type = type;
    }

    UIElementType getType() {
        return this->type;
    }
protected:
    UIElementState state = DEFAULT;
    UIElementType type = UIELEMENT;
};