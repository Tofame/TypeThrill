#pragma once

#include <vector>
#include "UIElement.h"
#include "SFML/Graphics/RectangleShape.hpp"

class Panel : public UIElement {
public:
    Panel(sf::Vector2f size, sf::Vector2f position);
    Panel(UIElement* parent, sf::Vector2f size, sf::Vector2f posRatios);

    ~Panel();

    void draw() override;
    void update() override;
    void handleClick() override {};

    bool isClicked(const sf::Vector2i& mousePos) override { return false;};

    std::vector<UIElement*> UIElements;

    void addElement(UIElement* ptrUIElement);
    void removeElement(UIElement* ptrUIElement);
    UIElement* getElement(int index);


};