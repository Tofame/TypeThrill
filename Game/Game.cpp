#include "Game.h"

#include "GameInterface.h"
#include "../UI/ButtonFactory.h"
#include "fmt/core.h"

auto event = sf::Event();
Game::GameStates Game::gameState = Game::STATE_MENU;

void Game::run() {
    window.clear();
    // What is drawn depends on actual game state.
    switch(Game::getGameState()) {
        case STATE_PLAYING:
            // GameInterface here will draw && GameLogic will move everything (words etc)
            break;
        case STATE_MENU:
            GameInterface::drawMenu();
            break;
        case STATE_PAUSED:
            // Unsure whether to implement it at all
            break;
        case STATE_GAMEOVER:
            // GameInterface draw GameOver -> a popup message "60s (or other set time in settings/menu) have passed"
            break;
        default:
            break;
    }

    // Handling window events (such as keyboard pressing)
    while (window.pollEvent(event)){
        switch(event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                window.setSize(originalSize);
            case sf::Event::KeyPressed:
                // Restart the game when gameover
                if(Game::getGameState() == STATE_GAMEOVER) {
                    resetGame();
                    continue;
                }

                switch(event.key.code) {
                    // Here we will be checking keyboard input (<= 127 and like BACKSPACE AND DELETE)
                    // We will have text iput field handled here...
                    default:
                        break;
                }
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    Button* buttonClicked = nullptr;
                    for (const auto& buttonPair : ButtonFactory::Buttons) {
                        if(buttonPair.second->isClicked(mousePos) == true) {
                            buttonClicked = buttonPair.second.get();
                            break;
                        }
                    }
                    if(buttonClicked) {
                        buttonClicked->handleClick();
                    }
                }
            default:
                break;
        }
    }

    window.display();
}

void Game::resetGame() {

}



void Game::setGameState(GameStates state) {
    switch(state) {
        case STATE_MENU:
        {
            Game::gameState = state;
            break;
        }
        case STATE_PLAYING:
        {
            Game::gameState = state;
            break;
        }
        case STATE_PAUSED:
        {
            Game::gameState = state;
            break;
        }
        case STATE_GAMEOVER:
        {
            Game::gameState = state;
            break;
        }
        default:
            throw std::runtime_error("An unknown GameState failed to be set: " + state);
    }
}

Game::GameStates Game::getGameState() {
    return Game::gameState;
}
