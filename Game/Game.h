#pragma once

#include "../Globals.h"

class Game {
    public:
        void static run();

        void static handleMousePress(sf::Mouse::Button mouseButton);
        void static checkUIElementsForClick(sf::Vector2i mousePos);
        void static resetGame();

        enum GameStates {
            STATE_MENU = 0,
            STATE_PLAYING = 1,
            STATE_PAUSED = 2,
            STATE_GAMEOVER = 3
        };

        void static setGameState(GameStates state);
        GameStates static getGameState();

        void static handleTextEntered(sf::Uint32 unicode);
    private:
        static GameStates gameState;
};