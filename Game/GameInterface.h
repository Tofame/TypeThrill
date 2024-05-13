#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"

class GameInterface {
public:
    enum MenuStates {
        MENU_DEFAULT,
        MENU_SETTINGS,
        MENU_LOAD,
        MENU_HIGHSCORES
    };

    void static drawMenu();
    void static drawMenuBackground();
    void static drawMenuWindow();
    void static drawMenuButtons();

    void static setMenuState(MenuStates state);
    MenuStates static getMenuState();

    void static setupUI();
    void static setupBackgroundSprite();
    void static setupGameTitle();

    void static setBackgrundSprite(sf::Sprite& sprite);
    sf::Sprite static getBackgroundSprite();

    void static setGameTitle(sf::Text& text);
    sf::Text static getGameTitle();
private:
    MenuStates static menuState;
    sf::Sprite static backgroundSprite;
    sf::Texture static backgroundTexture; // texture has to live as long as a sprite so it cant be "created" in a method
    sf::Text static gameTitle;
};
