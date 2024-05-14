#pragma once

#include <vector>
#include "UIElement.h"
#include "SFML/Graphics/RectangleShape.hpp"

class Panel {
public:
    Panel(const sf::Vector2f& size, const sf::Vector2f& position);
    Panel(const sf::Vector2f& size);

    void draw();
    void update();

    std::vector<UIElement*> UIElements;

    void addElement(UIElement* ptrUIElement);
    UIElement& getElement(int index) const;
private:
    sf::RectangleShape body;
};
