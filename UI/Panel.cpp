#include "Panel.h"
#include "../Globals.h"

#include <stdexcept>

#include "../Settings.h"

auto UIElements = std::vector<UIElement*>();

Panel::Panel(sf::Vector2f size, sf::Vector2f position) {
    this->visibility = false;

    this->body = sf::RectangleShape(size);
    this->body.setScale(1.0, 1.0);

    this->body.setPosition(position);
}

Panel::Panel(UIElement* parent, sf::Vector2f size, sf::Vector2f posRatios) {
    this->parent = parent;

    this->visibility = false;

    this->body = sf::RectangleShape(size * Settings::getUIScale());
    this->body.setScale(1.0, 1.0);
    this->setPosRatios(posRatios.x, posRatios.y);

    this->body.setPosition(parent->body.getSize().x * posXRatio - body.getSize().x/2, parent->body.getSize().y * posYRatio - body.getSize().y/2);
}

Panel::~Panel() {
    for(UIElement* uielement : UIElements) {
        delete uielement;
    }
}

void Panel::draw() {
    if(this->parent == nullptr) return;

    window.draw(this->body);

    for(UIElement* uielement : UIElements) {
        uielement->draw();
    }
}

void Panel::update() {
    if (this->parent == nullptr) { // panelWindow is a panel with no parent, its the "father" of all panels
        this->body.setScale({1.0f, 1.0f});
        sf::FloatRect globalBounds = this->body.getGlobalBounds();

        if (globalBounds.width > window.getSize().x || globalBounds.height > window.getSize().y) {
            float scaleX = window.getSize().x / globalBounds.width;
            float scaleY = window.getSize().y / globalBounds.height;
            float scaleFactor = std::min(scaleX, scaleY);
            this->body.setScale(scaleFactor, scaleFactor);
        }
    } else {
        this->body.setScale(this->parent->body.getScale());
    }

    sf::Vector2f newPosition(
        this->posXRatio * window.getSize().x - (this->body.getGlobalBounds().width) / 2,
        this->posYRatio * window.getSize().y - (this->body.getGlobalBounds().height) / 2
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

void Panel::removeElement(UIElement* UIElement) {
    auto it = std::find(this->UIElements.begin(), UIElements.end(), UIElement);
    if (it != UIElements.end()) {
        delete *it;
        UIElements.erase(it);
    }
}

UIElement* Panel::getElement(int index) {
    if (index < 0 || index >= UIElements.size()) {
        throw std::out_of_range("Index " + std::to_string(index) + " out of range in Panel::getElement");
    }
    return this->UIElements.at(index);
}

void Panel::setType(PanelType type) {
    this->type = type;
}

PanelType Panel::getType() {
    return this->type;
}