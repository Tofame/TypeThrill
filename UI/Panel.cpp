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
    this->body.setPosition(window.getSize().x * posXRatio - body.getSize().x/2, window.getSize().y * posYRatio);
}

void Panel::draw() {
    if(this->parent == nullptr) return;

    window.draw(this->body);

    for(UIElement* uielement : UIElements) {
        uielement->draw();
    }
}

void Panel::update() {

}

void Panel::addElement(UIElement* ptrUIElement) {
    this->UIElements.push_back(ptrUIElement);
}

UIElement& Panel::getElement(int index) const {
    if (index < 0 || index >= UIElements.size()) {
        throw std::out_of_range("Index out of range in Panel::getElement");
    }
    return *this->UIElements.at(index);
}