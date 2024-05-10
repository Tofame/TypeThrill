#include "GameInterface.h"

#include <stdexcept>

#include "../UI/ButtonFactory.h"

GameInterface::MenuStates GameInterface::menuState = GameInterface::MENU_DEFAULT;

void GameInterface::drawMenu() {
    for (const auto& buttonPair : ButtonFactory::Buttons) {
        buttonPair.second->draw();
    }
}

void GameInterface::setMenuState(MenuStates state) {
    switch(state) {
        case MENU_DEFAULT:
        {
            GameInterface::menuState = state;
            break;
        }
        case MENU_SETTINGS:
        {
            GameInterface::menuState = state;
            break;
        }
        case MENU_LOAD:
        {
            GameInterface::menuState = state;
            break;
        }
        case MENU_HIGHSCORES:
        {
            GameInterface::menuState = state;
            break;
        }
        default:
            throw std::runtime_error("An unknown MenuState failed to be set: " + state);
    }
}

GameInterface::MenuStates GameInterface::getMenuState() {
    return GameInterface::menuState;
}