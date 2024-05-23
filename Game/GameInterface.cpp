#include <cmath>

#include "../Globals.h"
#include "../ResourceManagers/FontManager.h"
#include "../Settings.h"
#include "GameInterface.h"

#include "Game.h"
#include "../UI/UIElementFactory.h"
#include <iostream>

#include "fmt/chrono.h"

GameInterface::MenuStates GameInterface::menuState = GameInterface::MENU_DEFAULT;
sf::Sprite GameInterface::backgroundSprite = sf::Sprite();
sf::Texture GameInterface::backgroundTexture = sf::Texture();
sf::Text GameInterface::gameTitle = sf::Text();

std::vector<Panel*> GameInterface::panels = std::vector<Panel*>();

void GameInterface::drawMenu() {
    GameInterface::drawMenuBackground();
    GameInterface::drawMenuWindow();
    GameInterface::drawPanels();
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


void GameInterface::drawPanels() {
    for (auto panel : GameInterface::panels) {
        if(panel->isVisible())
            panel->draw();
    }
}

void GameInterface::setMenuState(MenuStates state) {
    //auto oldState = getMenuState();

    // Hide all panels and in switch we will show whatever panels should be visible
    for (auto panel : GameInterface::panels) {
        panel->setVisibility(false);
    }

    switch(state) {
        case MENU_DEFAULT:
        {
            auto panelToShow = getPanelByType(PANEL_MENU);
            if(panelToShow != nullptr)
                panelToShow->setVisibility(true);
            GameInterface::menuState = state;
            break;
        }
        case MENU_SETTINGS:
        {
            auto panelToShow = getPanelByType(PANEL_SETTINGS);
            if(panelToShow != nullptr)
                panelToShow->setVisibility(true);
            GameInterface::menuState = state;
            break;
        }
        case MENU_LOAD:
        {
            auto panelToShow = getPanelByType(PANEL_LOADGAME);
            if(panelToShow != nullptr)
                panelToShow->setVisibility(true);
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
    GameInterface::setGameTitle(text);

    // Sets position of GameTitle
    GameInterface::updateGameTitle();
}

void GameInterface::updateGameTitle() {
    sf::Text text = GameInterface::getGameTitle();

    auto parentScale = panels[0]->body.getScale();
    text.setScale(parentScale);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);

    sf::Vector2f textCenter(
        (window.getSize().x * 0.5),
        (window.getSize().y * 0.15)
    );

    text.setPosition(textCenter);

    GameInterface::setGameTitle(text);
}

void GameInterface::setupPanels() {
    auto panelWindow = new Panel({(float)originalWindowSize.x, (float)originalWindowSize.y}, {0,0});
    panelWindow->setType(PANEL_WINDOW);

    auto panelMenu = UIElementFactory::createPanel(panelWindow, {300, 400}, {0.5, 0.6}, PANEL_MENU);
    panelMenu->setVisibility(true);

    panelMenu->addElement(UIElementFactory::createMenuButton("New Game", []() -> void { Game::setGameState(Game::STATE_PLAYING); }));
    panelMenu->addElement(UIElementFactory::createMenuButton("Load Game", []() -> void { GameInterface::setMenuState(MENU_LOAD); }));
    panelMenu->addElement(UIElementFactory::createMenuButton("Settings", []() -> void { GameInterface::setMenuState(MENU_SETTINGS); }));
    panelMenu->addElement(UIElementFactory::createMenuButton("Exit", []() -> void { exit(0); }));
    int i = 0;
    for(auto uielement : panelMenu->UIElements) {
        uielement->setPosRatios(0.5, uielement->posYRatio + i * 0.24);
        i++;
    }

    auto panelSettings = UIElementFactory::createPanel(panelWindow, {600, 400}, {0.5, 0.5}, PANEL_SETTINGS);
    panelSettings->addElement(UIElementFactory::createMenuButton("Back", []() -> void { GameInterface::setMenuState(MENU_DEFAULT); }));

    GameInterface::addPanelToVector(panelWindow);
    GameInterface::addPanelToVector(panelMenu);
    GameInterface::addPanelToVector(panelSettings);

    GameInterface::updatePanels();
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
        if(panel->body.getPosition().x != 0 || panel->body.getPosition().y != 0) {
            throw std::runtime_error("Tried to execute `addPanelToVector` on index 0, while the first panel must be panelWindow (the only one with pos 0,0)");
        }
    }

    GameInterface::panels.push_back(panel);
}

void GameInterface::updatePanels() {
    for(auto panel : GameInterface::panels) {
        panel->update();
    }
}

Panel* GameInterface::getPanelByType(PanelType type) {
    for(auto panel : GameInterface::panels) {
        if(panel->getType() == type) {
            return panel;
        }
    }

    return nullptr;
}
