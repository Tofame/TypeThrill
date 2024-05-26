#include <cmath>

#include "../Globals.h"
#include "../ResourceManagers/FontManager.h"
#include "../Settings.h"
#include "GameInterface.h"

#include "Game.h"
#include "GameStatistics.h"
#include "WordSpawner.h"
#include "../UI/TextLabel.h"
#include "../UI/UIElementFactory.h"
#include "../UI/Word.h"

#include "fmt/chrono.h"

GameInterface::MenuStates GameInterface::menuState = GameInterface::MENU_DEFAULT;
sf::Sprite GameInterface::backgroundSprite = sf::Sprite();
sf::Texture GameInterface::backgroundTexture = sf::Texture();
sf::Text GameInterface::gameTitle = sf::Text();

std::vector<Panel*> GameInterface::panels = std::vector<Panel*>();

void GameInterface::drawMenu() {
    GameInterface::drawMenuBackground();
    GameInterface::drawPanels();
}

void GameInterface::drawMenuBackground() {
    window.draw(GameInterface::getBackgroundSprite());
    window.draw(GameInterface::getGameTitle());
}

void GameInterface::drawPanels() {
    for (auto panel : GameInterface::panels) {
        if(panel->isVisible())
            panel->draw();
    }
}

void GameInterface::hideAllPanels() {
    for (auto panel : GameInterface::panels) {
        panel->setVisibility(false);
    }
}

void GameInterface::setMenuState(MenuStates state) {
    // We wont make a check for getState != state here as its called in Game:setGameState and we need it.

    // Hide all panels and in switch we will show whatever panels should be visible
    GameInterface::hideAllPanels();

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
    text.setOrigin(textBounds.left + textBounds.width/2, textBounds.top + textBounds.height/2);

    sf::Vector2f textCenter(
        (window.getSize().x * 0.5),
        (window.getSize().y * 0.1)
    );

    text.setPosition(textCenter);

    GameInterface::setGameTitle(text);
}

void GameInterface::setupPanels() {
    // ================= Setting up the Window Panel
    auto panelWindow = new Panel({(float)originalWindowSize.x, (float)originalWindowSize.y}, {0,0});
    panelWindow->setType(PANEL_WINDOW);

    // ================= Setting up the Menu Panel
    auto panelMenu = UIElementFactory::createPanel(panelWindow, {300, 400}, {0.5, 0.45}, PANEL_MENU);
    panelMenu->setVisibility(true);

    panelMenu->addElement(UIElementFactory::createMenuButton("New Game", []() -> void { GameStatistics::setupDefaultStatistics(); Game::setGameState(Game::STATE_PLAYING); }, {0.5, 0.15}));
    panelMenu->addElement(UIElementFactory::createMenuButton("Load Game", []() -> void { GameInterface::setMenuState(MENU_LOAD); }, {0.5, 0.15}));
    panelMenu->addElement(UIElementFactory::createMenuButton("Settings", []() -> void { GameInterface::setMenuState(MENU_SETTINGS); }, {0.5, 0.15}));
    panelMenu->addElement(UIElementFactory::createMenuButton("Exit", []() -> void { exit(0); }, {0.5, 0.15}));
    int i = 0;
    for(auto uielement : panelMenu->UIElements) {
        uielement->setPosRatios(0.5, uielement->posYRatio + i * 0.24);
        i++;
    }

    // ================= Setting up the Settings Panel
    auto panelSettings = UIElementFactory::createPanel(panelWindow, {700, 600}, {0.5, 0.30}, PANEL_SETTINGS);

    auto settingsTextLabel = new TextLabel("Remember, always click 'Apply' to apply changes.", {0.5, 0.75});
    panelSettings->addElement(settingsTextLabel);
    settingsTextLabel->update();

    auto wordSpeedField = UIElementFactory::createTextField(
        fmt::format("{:.5f}", Settings::getWordsSpeed()),
        {0.01, 0.05},
        "Word Speed",
        L"^[0-9]([.,][0-9]{0,5})?$" // L casts to wchar as we need wstring in wregex
    );
    panelSettings->addElement(wordSpeedField);

    auto wordFrequencyField = UIElementFactory::createTextField(
        fmt::format("{:.2f}", Settings::getWordsFrequency()),
        {0.01, 0.15},
        "Word Frequency",
        L"^[0-9]([.,][0-9]{0,2})?$"
    );
    panelSettings->addElement(wordFrequencyField);

    auto wordSize = UIElementFactory::createTextField(
        fmt::format("{:.2f}", Settings::getWordsSize()),
        {0.01, 0.25},
        "Word Size",
        L"^[0-9]([.,][0-9]{0,2})?$"
    );
    panelSettings->addElement(wordSize);

    auto wordHighlight = UIElementFactory::createMenuCheckbox(
        1.0,
        {0.01, 0.35},
        "Word Highlight",
        Settings::isWordsHighlightEnabled()
    );
    panelSettings->addElement(wordHighlight);

    auto UIScaleSetting = UIElementFactory::createTextField(
        fmt::format("{:.2f}", Settings::getUIScale()),
        {0.01, 0.55},
        "UI Scale (max 5.99)",
        L"^[0-5]([.,][0-9]{0,2})?$"
    );
    panelSettings->addElement(UIScaleSetting);

    panelSettings->addElement(UIElementFactory::createMenuButton("Back", []() -> void { GameInterface::setMenuState(MENU_DEFAULT); }, {0.1, 0.94}, {100, 40}));
    panelSettings->addElement(UIElementFactory::createMenuButton("Restore Default", []() -> void { Settings::restoreDefaultSettings(); }, {0.4, 0.94}, {200, 40}));
    panelSettings->addElement(UIElementFactory::createMenuButton("Apply", []() -> void { Settings::applySettingsPanel(); }, {0.65, 0.94}, {100, 40}));

    // ================= Setting up the Panels for the actual Game (WORDS and GAMESTATISTICS)
    auto panelWords = UIElementFactory::createPanel(panelWindow, {1700, 500}, {0.5, 0.20}, PANEL_WORDS);
    panelWords->body.setOutlineThickness(4);
    panelWords->body.setFillColor(sf::Color(153, 153, 153, 60));

    auto panelGameStatistics = UIElementFactory::createPanel(panelWindow, {1700, 280}, {0.5, 0.67}, PANEL_GAMESTATISTICS);
    panelGameStatistics->body.setOutlineThickness(4);
    panelGameStatistics->body.setOutlineColor(sf::Color(219, 170, 44));
    panelGameStatistics->body.setFillColor(sf::Color(153, 153, 153, 60));

    auto gameInputWords = UIElementFactory::createTextField(
        "",
        {0.5, 0.5},
        "",
        L".{0,35}", // Anything can be in there as its up to user what he types
        []() -> void { WordSpawner::manageWords(); }
    );
    gameInputWords->body.setSize({420, 30});
    gameInputWords->setState(FOCUSED_ALWAYS);
    panelGameStatistics->addElement(gameInputWords);

    // Adding each Panel to Panels Vector
    GameInterface::addPanelToVector(panelWindow);
    GameInterface::addPanelToVector(panelMenu);
    GameInterface::addPanelToVector(panelSettings);
    GameInterface::addPanelToVector(panelWords);
    GameInterface::addPanelToVector(panelGameStatistics);

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
