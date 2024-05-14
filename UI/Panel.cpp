#include "Panel.h"
#include "../Globals.h"

#include <stdexcept>

auto UIElements = std::vector<UIElement*>();

Panel::Panel(const sf::Vector2f &size, const sf::Vector2f &position) {
    this->body = sf::RectangleShape(size);
    this->body.setPosition(position);
}

Panel::Panel(const sf::Vector2f &size) {
    this->body = sf::RectangleShape(size);
    this->body.setPosition(window.getSize().x * 0.5 - body.getSize().x/2, window.getSize().y * 0.4);
}

void Panel::draw() {
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