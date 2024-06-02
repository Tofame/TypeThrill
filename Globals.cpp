#include "Globals.h"
#include <filesystem>

// GENERAL
sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
sf::RenderWindow window = sf::RenderWindow(desktopMode, "TypeThrill");
sf::Vector2u originalWindowSize = window.getSize();

sf::Vector2u windowSizeStandard = {1920, 1080};

std::string projectPath = std::filesystem::current_path().string();