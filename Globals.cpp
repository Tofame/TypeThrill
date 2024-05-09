#include "Globals.h"
#include <filesystem>

// GENERAL
sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
sf::RenderWindow window = sf::RenderWindow(desktopMode, "ThrillType");
const sf::Vector2u originalSize = window.getSize();

std::string projectPath = std::filesystem::current_path().parent_path().string();