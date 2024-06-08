#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "Globals.h"
#include "Settings.h"
#include "Game/Game.h"
#include "Game/GameInterface.h"
#include "Game/Highscores.h"
#include "ResourceManagers/FontManager.h"
#include "ResourceManagers/GameSaveManager.h"
#include "ResourceManagers/TextureManager.h"
#include "Translator/WordLocales.h"

int main() {
    // Always place resource loaders first, then loadSettings second  (the following classes use them)
    FontManager::loadFonts();
    WordLocales::loadLocales();

    Settings::getInstance()->preLoadSettings(true); // Setups the std::map (settingsMap)
    Settings::getInstance()->loadSettingsFromTxt(); // Loads values from Settings.txt
    Highscores::getInstance()->loadHighscores();
    GameInterface::getInstance()->setupUI();

    // Order of calling the method below doesn't matter, it just should be after setupUI (as it uses some panels)
    GameSaveManager::getInstance()->preloadInitialInformation();

    Game::getInstance()->setGameState(STATE_MENU, true);

    // Game loop that will work the same on different (weaker/stronger) computers
    while (window.isOpen()){
        auto frameStart = std::chrono::high_resolution_clock ::now();
        Game::getInstance()->run();
        auto frameEnd = std::chrono::high_resolution_clock ::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
        auto delay = std::chrono::milliseconds(1000) / 100 - frameDuration; // 100 fps

        if(delay > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(delay);
        }
    }

    return 0;
}