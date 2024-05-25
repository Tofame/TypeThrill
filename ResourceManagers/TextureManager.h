#pragma once

#include "../Globals.h"

class TextureManager {
public:
    static std::map<std::string, sf::Texture> Textures;
    static void loadTextures();
};