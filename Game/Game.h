#pragma once

#include "../Globals.h"

enum GameStates {
    STATE_MENU = 0,
    STATE_NEWGAMESETUP = 1,
    STATE_PLAYING = 2,
    STATE_PAUSED = 3,
    STATE_GAMEOVER = 4
};

// https://refactoring.guru/design-patterns/singleton/cpp/example
// We go with Naive Singleton for now
class Game {
protected:
    Game() = default;
    static Game* game_;

public:
    // Singleton is not cloneable
    Game(Game& other) = delete;
    // Singleton can't be assigned
    void operator=(const Game&) = delete;

    static Game* getInstance();

    void run();

    void handleMousePress(sf::Mouse::Button mouseButton);
    void checkUIElementsForClick(sf::Vector2i mousePos);

    void setGameState(GameStates state, bool hidePanels);
    GameStates getGameState();

    void handleTextEntered(sf::Uint32 unicode);

    void onGameOver();

    void pause();

    void backToMenu();
private:
    GameStates gameState;
};