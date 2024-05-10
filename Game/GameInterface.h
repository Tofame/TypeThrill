#pragma once
#include <map>
#include "../UI/Button.h"

class GameInterface {
public:
    enum MenuStates {
        MENU_DEFAULT,
        MENU_SETTINGS,
        MENU_LOAD,
        MENU_HIGHSCORES
    };

    void static drawMenu();

    void static setMenuState(MenuStates state);
    MenuStates static getMenuState();
private:
    MenuStates static menuState;
};
