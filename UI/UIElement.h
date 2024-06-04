#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "../Misc/PosRatio.h"

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

    sf::RectangleShape body;
    PosRatio posRatio = PosRatio();

    virtual ~UIElement() = default;

    virtual void draw() {};
    virtual void update() {};
    virtual void handleClick() {};
    virtual void onWriteableKeyPressed(int mode, sf::Uint32 c) {}; // Writeable means backspace/space/delete and alphabet

    virtual bool isMouseOver(const sf::Vector2i& mousePos) { return false; };

    virtual void move(float x, float y) {};
    virtual void setPosition(float x, float y) {};

    void setParent(UIElement* parent);
    UIElement* getParent();

    bool isVisible();
    void setVisibility(bool value);
    void toggleVisibility(bool value);

    bool isEnabled();
    void setEnabled(bool value);

    virtual void setState(UIElementState state);
    UIElementState getState();

    void setType(UIElementType type);
    UIElementType getType();

protected:
    UIElement* parent = nullptr;

    bool visibility = true;
    bool enabled = true;

    UIElementState state = DEFAULT;
    UIElementType type = UIELEMENT;
};