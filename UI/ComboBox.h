#pragma once

#include <functional>
#include <vector>

#include "Button.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

class ComboBox : public UIElement {
public:
    ComboBox(sf::Vector2f size, sf::Vector2f posRatios);
    ComboBox(UIElement* parent, sf::Vector2f size, sf::Vector2f posRatios);

    ~ComboBox() override;

    void draw() override;
    void update() override;
    void handleClick() override;

    bool isMouseOver(const sf::Vector2i& mousePos) override;

    std::vector<Button*> UIElements = std::vector<Button*>();

    void addRadioButton(std::string buttonText, std::function<void()> const& onClick);

    void addElement(Button* ptrUIElement);
    void removeElement(Button* ptrUIElement);
    Button* getElement(int index);

    void activate();
    void deactivate();
    bool isActive();

    sf::String getChosenTextString();
    void setChosenText(sf::String value);
    void setChosenText(sf::Text value);

    sf::Text& getText();
    void setText(sf::Text& text);
private:
    sf::Text text;
    sf::Text chosenText;
    bool active = false;
};
