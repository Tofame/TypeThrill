#include "TextureManager.h"

#include <filesystem>

// Method meant to simplify process of loading for example .png's
// Example use: loadResource("UI/checkbox.png");
sf::Texture TextureManager::loadResource(std::string path) {
    std::string pathToFile = projectPath + "/Resources/" + path;

    auto texture = sf::Texture();
    if(!texture.loadFromFile(pathToFile)) {
        throw std::runtime_error("Failed to TextureManager::loadResource in path: " + pathToFile);
    }

    return texture;
}