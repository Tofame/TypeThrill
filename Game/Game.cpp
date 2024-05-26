#include "Game.h"
#include "GameInterface.h"
#include "WordSpawner.h"
#include "../UI/Panel.h"
#include "fmt/core.h"

auto event = sf::Event();
Game::GameStates Game::gameState = Game::STATE_MENU;

void Game::run() {
    window.clear();
    // What is drawn depends on actual game state.
    switch(Game::getGameState()) {
        case STATE_PLAYING:
            // GameInterface here will draw && GameLogic will move everything (words etc)
            GameInterface::drawMenuBackground();
            GameInterface::drawPanels();
            WordSpawner::spawnWord();
            WordSpawner::moveWords();
            break;
        case STATE_MENU:
            GameInterface::drawMenu();
            break;
        case STATE_NEWGAMESETUP:
            GameInterface::drawMenuBackground();
            GameInterface::drawPanels();
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
                window.setView(sf::View(
                        sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)
                        ));

                GameInterface::updatePanels();
                GameInterface::updateGameTitle();
                break;
            case sf::Event::TextEntered:
                Game::handleTextEntered(event.text.unicode);
                break;
            case sf::Event::MouseButtonPressed:
                handleMousePress(event.mouseButton.button);
                break;
            case sf::Event::MouseMoved:
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                for(auto panel : GameInterface::panels) {
                    for(auto uielement : panel->UIElements) {
                        if(uielement->isMouseOver(mousePosition))
                        {
                            if(uielement->getState() == DEFAULT) {
                                uielement->setState(HOVERED);
                            }
                            break;
                        } else {
                            if(uielement->getState() == HOVERED) {
                                uielement->setState(DEFAULT);
                            }
                        }
                    }
                }

                break;
            }
            default:
                break;
        }
    }

    window.display();
}

void Game::handleMousePress(sf::Mouse::Button mouseButton) {
    if (mouseButton == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        checkUIElementsForClick(mousePos);
    }
}

void Game::checkUIElementsForClick(sf::Vector2i mousePos) {
    for (auto panel : GameInterface::panels) {
        if(panel->isVisible() == false)
            continue;

        for(auto uielement : panel->UIElements) {
            // We skip elements that are FOCUSED_ALWAYS (example of it is TextField used during game for typing)
            if(uielement->getState() == FOCUSED_ALWAYS)
                continue;

            if(uielement->isVisible() && uielement->isMouseOver(mousePos) == true) {
                uielement->handleClick();
                return;
            } else if(uielement->getState() == FOCUSED) {
                uielement->setState(DEFAULT);
            }
        }
    }
}

void Game::setGameState(GameStates state) {
    if(Game::getGameState() == state) {
        return;
    }

    GameInterface::hideAllPanels();

    switch(state) {
        case STATE_MENU:
        {
            GameInterface::setMenuState(GameInterface::MENU_DEFAULT);
            Game::gameState = state;
            break;
        }
        case STATE_PLAYING:
        {
            auto panelToShow = GameInterface::getPanelByType(PANEL_WORDS);
            if(panelToShow != nullptr)
                panelToShow->setVisibility(true);
            auto panelToShow2 = GameInterface::getPanelByType(PANEL_GAMESTATISTICS);
            if(panelToShow2 != nullptr)
                panelToShow2->setVisibility(true);
            Game::gameState = state;
            break;
        }
        case STATE_NEWGAMESETUP:
        {
            auto panelToShow = GameInterface::getPanelByType(PANEL_NEWGAMESETUP);
            if(panelToShow != nullptr) {
                panelToShow->setVisibility(true);
            }
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

void Game::handleTextEntered(sf::Uint32 unicode) {
    int mode = 0; //0-delete mode (backspace, delete),1-add mode (space and letter(s))

    if (unicode == 8) { // Backspace
        mode = 0;
    } else if (unicode >= 32) { // Handle other characters
        mode = 1;
    } else {
        return;
    }

    for(auto panel : GameInterface::panels) {
        if(panel->isVisible()) {
            for(auto uielement : panel->UIElements) {
                if(uielement->getState() == FOCUSED || uielement->getState() == FOCUSED_ALWAYS) {
                    uielement->onWriteableKeyPressed(mode, unicode);
                    break;
                }
            }
        }
    }
}
