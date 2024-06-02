#pragma once

#include "SFML/Graphics.hpp"

// GENERAL
extern sf::RenderWindow window;
extern sf::Vector2u originalWindowSize;

// All sizes defined for UIElements etc. were based on the 1920x1080 resolution
// That's why even though we have automatic scalling and even UIScale settings
// It's better to also have ratio of the of the model size.
extern sf::Vector2u windowSizeStandard;

extern std::string projectPath;