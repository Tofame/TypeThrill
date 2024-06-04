#include "UIElement.h"

void UIElement::setParent(UIElement* parent) { this->parent = parent; };
UIElement* UIElement::getParent() { return parent; };

// Returns the value of 'visibility' field. It decides whether UIElement is drawn or not.
bool UIElement::isVisible() { return visibility; };
void UIElement::setVisibility(bool value) { visibility = value; };
void UIElement::toggleVisibility(bool value) { visibility = !visibility; };

// Returns the value of 'enabled' field. It decides whether UIElement is responsive
// This means that 'enabled' = false, will mean it can't be: clicked, written, checked etc.
bool UIElement::isEnabled() { return enabled; };
void UIElement::setEnabled(bool value) { enabled = value; };

void UIElement::setState(UIElementState state) {
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
UIElementState UIElement::getState() { return state; };


void UIElement::setType(UIElementType type) {
    this->type = type;
}
UIElementType UIElement::getType() {
    return this->type;
}