#include <cmath>

#include "../Globals.h"
#include "../ResourceManagers/FontManager.h"
#include "../Settings.h"
#include "GameInterface.h"

#include "Game.h"
#include "../UI/UIElementFactory.h"
#include <iostream>

GameInterface::MenuStates GameInterface::menuState = GameInterface::MENU_DEFAULT;
sf::Sprite GameInterface::backgroundSprite = sf::Sprite();
sf::Texture GameInterface::backgroundTexture = sf::Texture();
sf::Text GameInterface::gameTitle = sf::Text();

std::vector<Panel*> GameInterface::panels = std::vector<Panel*>();

void GameInterface::drawMenu() {
    GameInterface::drawMenuBackground();
    GameInterface::drawMenuWindow();
    GameInterface::drawMenuButtons();
}

void GameInterface::drawMenuBackground() {
    window.draw(GameInterface::getBackgroundSprite());
    window.draw(GameInterface::getGameTitle());
}

void GameInterface::drawMenuWindow() {
    switch(getMenuState()) {
        case MENU_SETTINGS:
        {
            break;
        }
        default:
            break;
    }
}


void GameInterface::drawMenuButtons() {
    for (Panel* panel : GameInterface::panels) {
        panel->draw();
    }
}

void GameInterface::setMenuState(MenuStates state) {
    switch(state) {
        case MENU_DEFAULT:
        {
            GameInterface::menuState = state;
            break;
        }
        case MENU_SETTINGS:
        {
            GameInterface::menuState = state;
            break;
        }
        case MENU_LOAD:
        {
            GameInterface::menuState = state;
            break;
        }
        case MENU_HIGHSCORES:
        {
            GameInterface::menuState = state;
            break;
        }
        default:
            throw std::runtime_error("An unknown MenuState failed to be set: " + state);
    }
}

GameInterface::MenuStates GameInterface::getMenuState() {
    return GameInterface::menuState;
}

// SFML also provides a way to draw to a texture instead of directly to a window: https://www.sfml-dev.org/tutorials/2.6/graphics-draw.php
void GameInterface::setupBackgroundSprite() {
    auto BG_BasisTexture = sf::Texture();
    if (!BG_BasisTexture.loadFromFile(projectPath + "/Resources/UI/titleBackground.png"))
        throw std::runtime_error("Couldnt load Texture for setupBackgroundTexture()");
    auto tempBGSprite = sf::Sprite(BG_BasisTexture);

    // We will be using backgroundBasisTexture which is smaller than the screen size, and draw multiple of them
    // into RenderTexture to make it as big as user's screen
    auto BG_RenderTexture = sf::RenderTexture();
    if (!BG_RenderTexture.create(window.getSize().x, window.getSize().y))
        throw std::runtime_error("Couldnt create RenderTexture for setupBackgroundTexture()");

    auto BG_BasisTextureWidth = BG_BasisTexture.getSize().x;
    auto BG_BasisTextureHeight = BG_BasisTexture.getSize().y;

    // Ceil wont work if its double divided by int
    for(auto row = 0; row < std::ceil(window.getSize().x/(double)BG_BasisTextureWidth); row++) {
        for(auto column = 0; column < std::ceil(window.getSize().y/(double)BG_BasisTextureHeight); column++) {
            tempBGSprite.setPosition(row * BG_BasisTextureWidth, column * BG_BasisTextureHeight);
            BG_RenderTexture.draw(tempBGSprite);
        }
    }
    // Update backgroundTexture
    BG_RenderTexture.display();
    GameInterface::backgroundTexture = BG_RenderTexture.getTexture();

    GameInterface::backgroundSprite.setTexture(GameInterface::backgroundTexture);
}

void GameInterface::setupGameTitle() {
    auto text = sf::Text();
    text.setFont(FontManager::Fonts["voye"]);
    text.setCharacterSize(GameInterface::hugeCharacterSize * Settings::getUIScale());
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    text.setString("TYPE THRILL");
    text.setPosition(window.getSize().x/2 - text.getGlobalBounds().width/2, window.getSize().y * 0.1);
    GameInterface::setGameTitle(text);
}

void GameInterface::updateGameTitle() {
    sf::Text text = GameInterface::getGameTitle();

    unsigned int newCharacterSize = static_cast<unsigned int>(GameInterface::hugeCharacterSize * Settings::getUIScale() * (window.getSize().y / static_cast<float>(originalWindowSize.y)));
    text.setCharacterSize(newCharacterSize);

    // Recalculate the text bounds and update the origin to center the text
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);

    // Update the position of the text to ensure it remains centered
    text.setPosition(window.getSize().x / 2.0f, window.getSize().y * 0.1f);

    GameInterface::setGameTitle(text);
}

void GameInterface::setupPanels() {
    auto panelWindow = new Panel(nullptr, {(float)originalWindowSize.x, (float)originalWindowSize.y});
    panelWindow->body.setPosition(0,0);

    auto panelMenu = new Panel(panelWindow, {300, 400});
    panelMenu->addElement(UIElementFactory::createMenuButton("Play", []() -> void { Game::setGameState(Game::STATE_PLAYING); }));

    GameInterface::addPanelToVector(panelWindow);
    GameInterface::addPanelToVector(panelMenu);
}

void GameInterface::setBackgrundSprite(sf::Sprite& sprite) {
    GameInterface::backgroundSprite = sprite;
}

sf::Sprite GameInterface::getBackgroundSprite() {
    return GameInterface::backgroundSprite;
}

void GameInterface::setGameTitle(sf::Text &text) {
    GameInterface::gameTitle = text;
}

sf::Text GameInterface::getGameTitle() {
    return GameInterface::gameTitle;
};

void GameInterface::setupUI() {
    GameInterface::setupPanels();
    GameInterface::setupBackgroundSprite();
    GameInterface::setupGameTitle();
}

void GameInterface::addPanelToVector(Panel* panel) {
    if(GameInterface::panels.empty()) {
        if(panel->body.getPosition().x != 0 && panel->body.getPosition().y != 0) {
            throw std::runtime_error("Tried to execute `addPanelToVector` on index 0, while the first panel must be panelWindow (the only one with pos 0,0)");
        }
    }

    GameInterface::panels.push_back(panel);
}

void GameInterface::updatePanels() {
    for(Panel* panel : GameInterface::panels) {
        panel->update();
    }
}