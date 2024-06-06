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

    void addElementToHide(UIElement* element);
    void removeElementToHide(UIElement* element);

    std::vector<Button*> comboButtons = std::vector<Button*>();
    void addComboButton(std::string buttonText, std::function<void()> const& onClick);

    void activate();
    void deactivate();
    bool isActive();

    sf::String getChosenTextString();
    void setChosenText(sf::String value);
    void setChosenText(sf::Text value);

    sf::Text& getText();
    void setText(sf::Text& text);
protected:
    void addElement(Button* ptrUIElement);
    void removeElement(Button* ptrUIElement);
    Button* getElement(int index);

private:
    sf::Text text;
    sf::Text chosenText;
    bool active = false;

    // elementsToHide is a vector that stores elements that could overlap with radio buttons
    // of a combobox. elementsToHide's purpose is to hide those elements when activate()/deactivate()
    // methods are called
    std::vector<UIElement*> elementsToHide = std::vector<UIElement*>();
};