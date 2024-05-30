#pragma once

#include "../Globals.h"

class Game {
    public:
        void static run();

        void static handleMousePress(sf::Mouse::Button mouseButton);
        void static checkUIElementsForClick(sf::Vector2i mousePos);

        enum GameStates {
            STATE_MENU = 0,
            STATE_NEWGAMESETUP = 1,
            STATE_PLAYING = 2,
            STATE_PAUSED = 3,
            STATE_GAMEOVER = 4
        };

        void static setGameState(GameStates state, bool hidePanels);
        GameStates static getGameState();

        void static handleTextEntered(sf::Uint32 unicode);

        void static onGameOver();
    private:
        static GameStates gameState;
};