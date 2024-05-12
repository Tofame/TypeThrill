#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <chrono>

#include "Globals.h"
#include "Settings.h"
#include "Game/Game.h"
#include "Game/GameInterface.h"
#include "UI/ButtonFactory.h"

auto run() -> void;

int main() {
    fmt::println("Hello TypeThrill!");
    // Load Settings must always have top priority
    Settings::loadSettings();
    GameInterface::setupUI();
    Game::setGameState(Game::STATE_MENU);
    ButtonFactory::setupButtons();

    // Implement delta time (Game loop)
    // Source of knowledge - RyiSnow (https://youtu.be/VpH33Uw-_0E?list=PL_QPQmz5C6WUF-pOQDsbsKbaBZqXj4qSq&t=1072)
    double drawInterval = 1000000000 / 90; // 1000000000 is 1 second (billion nanoseconds)
    double delta = 0;
    long long lastTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    long long currentTime;
    long long timer = 0;

    while (window.isOpen()){
        currentTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        delta += (currentTime - lastTime) / drawInterval;
        timer += currentTime - lastTime;
        lastTime = currentTime;

        if (delta >= 1) {
            Game::run();
            delta--;
        }

        if (timer >= 1000000000 / 4) { // runs every quarter of a second
            // Some methods
            timer = 0;
        }
    }

    return 0;
}
