#pragma once
#include "../UI/Panel.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"

class GameInterface {
public:
    int inline static hugeCharacterSize = 140;
    int inline static mediumCharacterSize = 74;
    int inline static smallCharacterSize = 24;

    std::vector<Panel*> static panels;

    enum MenuStates {
        MENU_DEFAULT,
        MENU_SETTINGS,
        MENU_LOAD,
        MENU_HIGHSCORES
    };

    void static drawMenu();
    void static drawMenuBackground();
    void static drawPanels();

    void static hideAllPanels();

    void static setMenuState(MenuStates state);
    MenuStates static getMenuState();

    void static setupUI();
    void static setupBackgroundSprite();
    void static setupGameTitle();
    void static setupPanels();

    void static addPanelToVector(Panel* panel);
    void static updatePanels();
    static Panel* getPanelByType(PanelType type);

    void static setBackgrundSprite(sf::Sprite& sprite);
    sf::Sprite static getBackgroundSprite();

    void static setGameTitle(sf::Text& text);
    sf::Text static getGameTitle();
    void static updateGameTitle();
private:
    MenuStates static menuState;
    sf::Sprite static backgroundSprite;
    sf::Texture static backgroundTexture; // texture has to live as long as a sprite so it cant be "created" in a method
    sf::Text static gameTitle;
};
