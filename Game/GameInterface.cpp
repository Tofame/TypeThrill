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
    text.setCharacterSize(140 * Settings::getUIScale());
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    text.setString("TYPE THRILL");
    text.setPosition(window.getSize().x/2 - text.getGlobalBounds().width/2, window.getSize().y * 0.1);
    GameInterface::setGameTitle(text);
}

void GameInterface::setupPanels() {
    // Panel Window is a panel that ALWAYS covers whole window and its mandatory for whole UI.
    auto panelWindow = Panel(nullptr, {(float)originalWindowSize.x, (float)originalWindowSize.y});
    panelWindow.body.setPosition(0,0);

    auto panelMenu = Panel(&panelWindow, {500, 300});
    panelMenu.addElement(
UIElementFactory::createMenuButton("Play", []() -> void { Game::setGameState(Game::STATE_PLAYING); })
    );
    GameInterface::panels.push_back(&panelWindow);
    GameInterface::panels.push_back(&panelMenu);
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

void GameInterface::updatePanels() {
    for(Panel* panel : GameInterface::panels) {
        if(panel->parent == nullptr) { // Window Panel
            panel->body.setScale(1.0,1.0);
            if(panel->body.getGlobalBounds().width>window.getSize().x) {
                panel->body.setScale(window.getSize().x / panel->body.getGlobalBounds().width, window.getSize().x / panel->body.getGlobalBounds().width);
            }
            if(panel->body.getGlobalBounds().height>window.getSize().y) {
                panel->body.setScale(window.getSize().y /panel->body.getGlobalBounds().height , window.getSize().y / panel->body.getGlobalBounds().height);
            }

            panel->body.setPosition(panel->posXRatio * window.getSize().x - panel->body.getSize().x/2, panel->posYRatio * window.getSize().y - panel->body.getSize().y/2);
            std::cout<<"aha: "<<window.getSize().x / panel->body.getGlobalBounds().width<<" "<<window.getSize().x / panel->body.getGlobalBounds().width<<"\n";
            std::cout<<"no parent:("<<panel->body.getScale().x<<", "<<panel->body.getScale().y<<")\n";
        } else {
            auto scaleParent = panel->parent->body.getScale();
            std::cout << "has parent: " << panel->parent->body.getScale().x << " " << panel->parent->body.getScale().y;
            panel->body.setScale(scaleParent);
            panel->body.setPosition(100,100);
            std::cout<<panel->body.getPosition().x<<" "<<panel->body.getPosition().y<<"\n";
            //panel.body.setPosition(panel.posXRatio * window.getSize().x - (panel.body.getSize().x * scaleParent.x)/2, panel.posYRatio * window.getSize().y - (panel.body.getSize().y * scaleParent.y)/2);
        }
    }
}