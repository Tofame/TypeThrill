#pragma once

#include <vector>
#include "UIElement.h"
#include "SFML/Graphics/RectangleShape.hpp"

enum PanelType {
    PANEL_WINDOW = 0,
    PANEL_MENU = 1,
    PANEL_SETTINGS = 2,
    PANEL_LOADGAME = 4,
    PANEL_HIGHSCORES = 5,
    PANEL_NEWGAMESETUP = 6,
    PANEL_WORDS = 7,
    PANEL_GAMESTATISTICS = 8,
};

class Panel : public UIElement {
public:
    Panel(sf::Vector2f size, sf::Vector2f position);
    Panel(UIElement* parent, sf::Vector2f size, sf::Vector2f posRatios);

    ~Panel();

    void draw() override;
    void update() override;
    void handleClick() override {};

    bool isMouseOver(const sf::Vector2i& mousePos) override;

    std::vector<UIElement*> UIElements;

    void addElement(UIElement* ptrUIElement);
    void removeElement(UIElement* ptrUIElement);
    UIElement* getElement(int index);

    void setPanelType(PanelType type);
    PanelType getPanelType();

private:
    PanelType panelType = PANEL_WINDOW;
};