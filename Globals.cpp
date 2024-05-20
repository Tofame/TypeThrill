#include "Globals.h"
#include <filesystem>

// GENERAL
sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
sf::RenderWindow window = sf::RenderWindow(desktopMode, "Type Thrill");
sf::Vector2u originalWindowSize = window.getSize();

std::string projectPath = std::filesystem::current_path().string();