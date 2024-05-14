#pragma once

class UIElement {
protected:
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void handleClick() = 0;
};