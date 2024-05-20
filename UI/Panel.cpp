#include "Panel.h"
#include "../Globals.h"

#include <stdexcept>

auto UIElements = std::vector<UIElement*>();

Panel::Panel(sf::Vector2f size, sf::Vector2f position) {
    this->body = sf::RectangleShape(size);
    this->body.setPosition(position);
    this->body.setScale(1.0, 1.0);
}

Panel::Panel(UIElement* parent, sf::Vector2f size) {
    this->parent = parent;
    this->body = sf::RectangleShape(size);
    this->body.setScale(1.0, 1.0);
    this->posXRatio = 0.5;
    this->posYRatio = 0.4;
    this->body.setPosition(window.getSize().x * posXRatio - body.getSize().x/2, window.getSize().y * posYRatio - body.getSize().y/2);
}

void Panel::draw() {
    if(this->parent == nullptr) return;

    window.draw(this->body);

    for(UIElement* uielement : UIElements) {
        uielement->draw();
    }
}

void Panel::update() {
    sf::Vector2f scale = {1.0f, 1.0f};

    if (this->parent == nullptr) { // panelWindow is a panel with no parent, its the "father" of all panels
        this->body.setScale(scale);

        sf::FloatRect localBounds = this->body.getLocalBounds();

        if (localBounds.width > window.getSize().x) {
            scale.x = window.getSize().x / localBounds.width;
            scale.y = scale.x; // Maintain aspect ratio
            this->body.setScale(scale);
        }

        if (localBounds.height > window.getSize().y) {
            scale.y = window.getSize().y / localBounds.height;
            scale.x = scale.y; // Maintain aspect ratio
            this->body.setScale(scale);
        }
    } else {
        scale = this->parent->body.getScale();
        this->body.setScale(scale);
    }

    sf::Vector2f newPosition(
        this->posXRatio * window.getSize().x - (scale.x * this->body.getLocalBounds().width) / 2,
        this->posYRatio * window.getSize().y - (scale.y * this->body.getLocalBounds().height) / 2
    );
    this->body.setPosition(newPosition);

    for(auto* uielement : this->UIElements) {
        uielement->update();
    }
}

void Panel::addElement(UIElement* ptrUIElement) {
    ptrUIElement->setParent(this);
    this->UIElements.push_back(ptrUIElement);
}

UIElement& Panel::getElement(int index) const {
    if (index < 0 || index >= UIElements.size()) {
        throw std::out_of_range("Index out of range in Panel::getElement");
    }
    return *this->UIElements.at(index);
}