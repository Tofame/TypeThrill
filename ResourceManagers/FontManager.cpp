#include "FontManager.h"

#include <filesystem>

std::map<std::string, sf::Font> FontManager::Fonts;

void FontManager::loadFonts() {
    std::string fontDirectory = projectPath + "/Resources/Fonts";

    // Source of knowledge: uni lecture and https://en.cppreference.com/w/cpp/filesystem/path (stem())
    for (const auto& entry : std::filesystem::directory_iterator(fontDirectory)) {
        // Is a font
        if (entry.is_regular_file() && entry.path().extension() == ".ttf") {
            sf::Font font;
            if (font.loadFromFile(entry.path().string())) {
                Fonts[entry.path().stem().string()] = font;
            } else {
                throw std::runtime_error("Error while loading font in FontManager::loadFonts: " + entry.path().string());
            }
        }
    }
}