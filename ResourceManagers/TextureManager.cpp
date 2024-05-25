#include "TextureManager.h"

#include <filesystem>

std::map<std::string, sf::Texture> TextureManager::Textures;

void TextureManager::loadTextures() {
    std::string fontDirectory = projectPath + "/Resources/UI";

    // Source of knowledge: uni lecture and https://en.cppreference.com/w/cpp/filesystem/path (stem())
    for (const auto& entry : std::filesystem::directory_iterator(fontDirectory)) {
        // Is a .png/.jpg file (well, technically we can do more but it doesnt matter)
        auto extension = entry.path().extension();
        if (entry.is_regular_file() && (extension == ".png" || extension == ".jpg")) {
            sf::Texture texture;
            if (texture.loadFromFile(entry.path().string())) {
                Textures[entry.path().stem().string()] = texture;
            } else {
                throw std::runtime_error("Error while loading texture in TextureManager::loadTexture: " + entry.path().string());
            }
        }
    }
}