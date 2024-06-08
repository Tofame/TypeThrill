#include "Game.h"
#include "GameInterface.h"
#include "GameStatistics.h"
#include "WordSpawner.h"
#include "Highscores.h"
#include "../Settings.h"
#include "../UI/Panel.h"
#include "fmt/core.h"
#include "../UI/TextLabel.h"

auto event = sf::Event();

Game* Game::game_ = nullptr;

Game* Game::getInstance() {
    if(game_ == nullptr){
        game_ = new Game();
        game_->gameState = STATE_MENU;
    }
    return game_;
}

void Game::run() {
    window.clear();
    // What is drawn/done depends on actual game state.
    switch(Game::getGameState()) {
        case STATE_PLAYING:
            GameInterface::getInstance()->drawMenuBackground();
            GameInterface::getInstance()->drawPanels();
            WordSpawner::getInstance()->spawnWord();
            WordSpawner::getInstance()->moveWords();
            break;
        case STATE_MENU:
            GameInterface::getInstance()->drawMenu();
            break;
        case STATE_NEWGAMESETUP:
            GameInterface::getInstance()->drawMenuBackground();
            GameInterface::getInstance()->drawPanels();
            break;
        case STATE_PAUSED:
            GameInterface::getInstance()->drawMenuBackground();
            GameInterface::getInstance()->drawPanels();
            break;
        case STATE_GAMEOVER:
            GameInterface::getInstance()->drawMenuBackground();
            GameInterface::getInstance()->drawPanels();
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

                GameInterface::getInstance()->updatePanels();
                GameInterface::getInstance()->updateGameTitle();
                break;
            case sf::Event::TextEntered:
                Game::handleTextEntered(event.text.unicode);
                break;
            case sf::Event::KeyPressed:
            {
                auto keyCode = event.key.code;
                if (keyCode == sf::Keyboard::Key::Escape) {
                    Game::pause();
                }

                // Clears game text field with LAlt + Shift OR LAlt + Tilde
                if(getGameState() == STATE_PLAYING) {
                    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ||
                        (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) && sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde))
                    {
                        WordSpawner::getInstance()->clearWords(false, true, true);
                    }
                }

                break;
            }
            case sf::Event::MouseButtonPressed:
                handleMousePress(event.mouseButton.button);
                break;
            case sf::Event::MouseMoved:
            {
                Game::onMouseMove();
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
    for (auto panel : GameInterface::getInstance()->panels) {
        if(!panel->isVisible() || !panel->isMouseOver(mousePos) ) {
            continue;
        }

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

void Game::setGameState(GameStates state, bool hidePanels) {
    if(Game::getGameState() == state) {
        return;
    }

    if(hidePanels)
        GameInterface::getInstance()->hideAllPanels();

    switch(state) {
        case STATE_MENU:
        {
            GameInterface::getInstance()->setMenuState(GameInterface::getInstance()->MENU_DEFAULT);
            Game::gameState = state;
            break;
        }
        case STATE_PLAYING:
        {
            auto panelToShow1 = GameInterface::getInstance()->getPanelByType(PANEL_WORDS);
            if(panelToShow1 != nullptr)
                panelToShow1->setVisibility(true);
            auto panelToShow2 = GameInterface::getInstance()->getPanelByType(PANEL_GAMESTATISTICS);
            if(panelToShow2 != nullptr)
                panelToShow2->setVisibility(true);
            Game::gameState = state;
            break;
        }
        case STATE_NEWGAMESETUP:
        {
            auto panelToShow = GameInterface::getInstance()->getPanelByType(PANEL_NEWGAMESETUP);
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
            auto panelToShow = GameInterface::getInstance()->getPanelByType(PANEL_GAMEOVER);
            if(panelToShow != nullptr) {
                panelToShow->setVisibility(true);
            }
            Game::gameState = state;
            break;
        }
        default:
            throw std::runtime_error("An unknown GameState failed to be set: " + state);
    }
}

GameStates Game::getGameState() {
    return Game::gameState;
}

// It is called when we are in-game and we type a word (from TextEntered sfml event)
void Game::handleTextEntered(sf::Uint32 unicode) {
    auto gameState = Game::getGameState();
    if(gameState == STATE_PAUSED || gameState == STATE_GAMEOVER)
        return;

    int mode = 0; //0-delete mode (backspace, delete),1-add mode (space and letter(s))

    if (unicode == 8) { // Backspace
        mode = 0;
    } else if (unicode >= 32) { // Handle other characters
        mode = 1;
    } else {
        return;
    }

    for(auto panel : GameInterface::getInstance()->panels) {
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

void Game::onGameStart() {
    Settings::getInstance()->loadSetingsOnNewGame();
    GameStatistics::getInstance()->setupDefaultStatistics();
    this->setGameState(STATE_PLAYING, true);
}

void Game::onGameOver() {
    auto GameOverPanel = GameInterface::getInstance()->getPanelByType(PANEL_GAMEOVER);
    if(GameOverPanel == nullptr) {
        throw std::runtime_error("Game::onGameOver() is called when PANEL_GAMEOVER doesn't exist (yet?).");
        return;
    }

    TextLabel* highscoreTextLabel = nullptr;

    // We update uielements, among them can be e.g. DynamicLabels
    for(auto uielement : GameOverPanel->UIElements) {
        uielement->update();
        if(uielement->getType() == TEXTLABEL) {
            highscoreTextLabel = static_cast<TextLabel*>(uielement);
        }
    }

    // Check highscores and update them if actual ones are higher
    auto highscoreCheck = Highscores::getInstance()->updateHighScores();
    // We decide the text of the TextLabel with returned value from pdateHighScores()
    if(highscoreTextLabel) {
        if(highscoreCheck == -1) {
            highscoreTextLabel->getText().setString("No new highscore!\nBetter Luck Next Time!");
        } else {
            // +1 because we index from 0 so it would be Top 0 normally
            highscoreTextLabel->getText().setString(fmt::format("New Highscore!\nYour score is Top {}", highscoreCheck+1));
        }
        highscoreTextLabel->update();
    }
}

void Game::onMouseMove() {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

    for(auto panel : GameInterface::getInstance()->panels) {
        if(!panel->isVisible() || !panel->isMouseOver(mousePosition) ) {
            continue;
        }

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
}

// Pauses the game if the conditions are met (pause STATE_PLAYING, 'unpause' STATE_PAUSED)
// Updates game statistics variable: 'stat_timeAtStart', because otherwise time would "pass" during the pause
// To make loading/saving of the game possible I think I will use: https://en.cppreference.com/w/cpp/chrono/system_clock
// to_time_t and from_time_t methods.
void Game::pause() {
    auto gameState = Game::getGameState();

    auto panelToShow = GameInterface::getInstance()->getPanelByType(PANEL_PAUSE);
    if(panelToShow == nullptr) {
        throw std::runtime_error("Error in Game::Pause() - PANEL_PAUSE cannot be found.");
    }

    // UNPAUSE
    if(gameState == STATE_PAUSED) {
        auto newTime = GameStatistics::getInstance()->getTimeSinceStartWithPause();
        GameStatistics::getInstance()->setTimePassedSinceStart(newTime);
        //fmt::println("newtime {}", std::chrono::system_clock::to_time_t(newTime));
        setGameState(STATE_PLAYING,false);
        panelToShow->setVisibility(false);
    // PAUSE
    } else if(gameState == STATE_PLAYING) {
        GameStatistics::getInstance()->setPauseTime(std::chrono::high_resolution_clock::now());
        setGameState(STATE_PAUSED, false);
        panelToShow->setVisibility(true);
    }
}

void Game::backToMenu() {
    auto state = getGameState();
    if(state == STATE_PAUSED || state == STATE_GAMEOVER || state == STATE_PLAYING) {
        WordSpawner::getInstance()->clearWords(true, false, true);
    }

    Game::setGameState(STATE_MENU, true);
    GameInterface::getInstance()->setMenuState(GameInterface::getInstance()->MENU_DEFAULT);
}
