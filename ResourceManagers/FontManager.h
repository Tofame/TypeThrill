#pragma once

#include "../Globals.h"

class FontManager {
public:
    static std::map<std::string, sf::Font> Fonts;
    static void loadFonts();
};