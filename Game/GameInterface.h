#pragma once
#include "../UI/Panel.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"

// https://refactoring.guru/design-patterns/singleton/cpp/example
// We go with Naive Singleton for now
class GameInterface {
protected:
    GameInterface();
    static GameInterface* gameInterface_;
public:
    // Singleton is not cloneable
    GameInterface(GameInterface& other) = delete;
    // Singleton can't be assigned
    void operator=(const GameInterface&) = delete;

    static GameInterface* getInstance();

    int inline static const hugeCharacterSize = 140;
    int inline static const bigCharacterSize = 54;
    int inline static const mediumCharacterSize = 38;
    int inline static const mediumLiteCharacterSize = 30;
    int inline static const smallCharacterSize = 24;

    std::vector<Panel*> panels;

    enum MenuStates {
        MENU_DEFAULT,
        MENU_SETTINGS,
        MENU_LOAD,
        MENU_HIGHSCORES
    };

    void drawMenu();
    void drawMenuBackground();
    void drawPanels();

    void hideAllPanels();

    void setMenuState(MenuStates state);
    MenuStates getMenuState();

    void setupUI();
    void setupBackgroundSprite();
    void setupGameTitle();
    void setupPanels();

    void addPanelToVector(Panel* panel);
    void updatePanels();
    Panel* getPanelByType(PanelType panelType);

    void setBackgroundSprite(sf::Sprite& sprite);
    sf::Sprite getBackgroundSprite();

    void setGameTitle(sf::Text& text);
    sf::Text getGameTitle();
    void updateGameTitle();
private:
    MenuStates menuState;
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture; // texture has to live as long as a sprite so it cant be "created" in a method
    sf::Text gameTitle;
};