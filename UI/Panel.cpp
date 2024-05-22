#include "Panel.h"
#include "../Globals.h"

#include <stdexcept>

#include "../Settings.h"

auto UIElements = std::vector<UIElement*>();

Panel::Panel(sf::Vector2f size, sf::Vector2f position) {
    this->body = sf::RectangleShape(size);
    this->body.setScale(1.0, 1.0);

    this->body.setPosition(position);
}

Panel::Panel(UIElement* parent, sf::Vector2f size, sf::Vector2f posRatios) {
    this->parent = parent;
    this->body = sf::RectangleShape(size * Settings::getUIScale());
    this->body.setScale(1.0, 1.0);
    this->setPosRatios(posRatios.x, posRatios.y);

    this->body.setPosition(parent->body.getSize().x * posXRatio - body.getSize().x/2, parent->body.getSize().y * posYRatio - body.getSize().y/2);
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
        sf::FloatRect globalBounds = this->body.getGlobalBounds();

        if (globalBounds.width > window.getSize().x) {
            this->body.setScale(window.getSize().x / globalBounds.width,window.getSize().x / globalBounds.width);
            globalBounds = this->body.getGlobalBounds();
        }

        if (globalBounds.height > window.getSize().y) {
            this->body.setScale(window.getSize().y / globalBounds.height,window.getSize().y / globalBounds.height);
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

void Panel::addElement(UIElement* UIElement) {
    UIElement->setParent(this);
    UIElement->update();
    this->UIElements.push_back(UIElement);
}

UIElement* Panel::getElement(int index) const {
    if (index < 0 || index >= UIElements.size()) {
        throw std::out_of_range("Index out of range in Panel::getElement");
    }
    return this->UIElements.at(index);
}