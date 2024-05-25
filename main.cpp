#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

#include "Globals.h"
#include "Settings.h"
#include "Game/Game.h"
#include "Game/GameInterface.h"
#include "ResourceManagers/FontManager.h"
#include "ResourceManagers/TextureManager.h"
#include "Translator/WordLanguages.h"

auto run() -> void;

int main() {
    fmt::println("Hello TypeThrill!");
    // Always place resource loaders first, then loadSettings second  (the following classes use them)
    FontManager::loadFonts();
    Settings::loadSettings();
    WordLanguages::loadLocales();
    GameInterface::setupUI();
    Game::setGameState(Game::STATE_MENU);

    while (window.isOpen()){
        auto frameStart = std::chrono::high_resolution_clock ::now();
        Game::run();
        auto frameEnd = std::chrono::high_resolution_clock ::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
        auto delay = std::chrono::milliseconds(1000) / 100 - frameDuration; // 100 fps

        if(delay > std::chrono::milliseconds(0)) {
            std::this_thread::sleep_for(delay);
        }
    }

    return 0;
}
