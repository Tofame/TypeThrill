#include <cmath>

#include "../Globals.h"
#include "../ResourceManagers/FontManager.h"
#include "../Settings.h"
#include "GameInterface.h"

#include "Game.h"
#include "GameStatistics.h"
#include "WordSpawner.h"
#include "../UI/ComboBox.h"
#include "../UI/DynamicTextLabel.h"
#include "../UI/TextLabel.h"
#include "../UI/UIElementFactory.h"
#include "../UI/Word.h"
#include "../Game/Highscores.h"
#include "../Translator/WordLocales.h"

#include "fmt/chrono.h"

GameInterface::MenuStates GameInterface::menuState = GameInterface::MENU_DEFAULT;
sf::Sprite GameInterface::backgroundSprite = sf::Sprite();
sf::Texture GameInterface::backgroundTexture = sf::Texture();
sf::Text GameInterface::gameTitle = sf::Text();

std::vector<Panel*> GameInterface::panels = std::vector<Panel*>();

void setupHighscoresPanel(Panel* panelWindow);

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
            auto panelToShow = getPanelByType(PANEL_HIGHSCORES);
            if(panelToShow != nullptr)
                panelToShow->setVisibility(true);
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
    text.setCharacterSize(GameInterface::hugeCharacterSize * Settings::getUIScale(false));
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    text.setString("TYPETHRILL");
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
    panelWindow->setPanelType(PANEL_WINDOW);

    // ================= Setting up the Menu Panel
    auto panelMenu = UIElementFactory::createPanel(panelWindow, {300, 440}, {0.5, 0.45}, PANEL_MENU);
    panelMenu->setVisibility(true);

    panelMenu->addElement(UIElementFactory::createMenuButton("New Game", []() -> void { Game::setGameState(Game::STATE_NEWGAMESETUP, true); }, {0.5, 0.10}));
    panelMenu->addElement(UIElementFactory::createMenuButton("Load Game", []() -> void { GameInterface::setMenuState(MENU_LOAD); }, {0.5, 0.10}));
    panelMenu->addElement(UIElementFactory::createMenuButton("Settings", []() -> void { GameInterface::setMenuState(MENU_SETTINGS); }, {0.5, 0.10}));
    panelMenu->addElement(UIElementFactory::createMenuButton("Highscores", []() -> void { GameInterface::setMenuState(MENU_HIGHSCORES); }, {0.5, 0.10}));
    panelMenu->addElement(UIElementFactory::createMenuButton("Exit", []() -> void { exit(0); }, {0.5, 0.10}));
    int i = 0;
    for(auto uielement : panelMenu->UIElements) {
        uielement->posRatio.setValues(0.5, (uielement->posRatio.getY() + i * 0.19) + 0.02);
        i++;
    }

    // ================= Setting up the Settings Panel
    auto panelSettings = UIElementFactory::createPanel(panelWindow, {700, 600}, {0.5, 0.30}, PANEL_SETTINGS);

    auto settingsTextLabel = new TextLabel("Click 'Save' to keep changes even after game is closed.", {0.5, 0.80});
    panelSettings->addElement(settingsTextLabel);

    auto wordSpeedField = UIElementFactory::createTextField(
        fmt::format("{:.5f}", Settings::getWordsSpeed(false)),
        {0.01, 0.05},
        "Word Speed",
        L"^[0-9]([.][0-9]{0,5})?$" // L casts to wchar as we need wstring in wregex
    );
    wordSpeedField->onTextFieldUpdate = [wordSpeedField]() -> void { Settings::setWordsSpeed(wordSpeedField->getInputString()); };
    panelSettings->addElement(wordSpeedField);

    auto wordFrequencyField = UIElementFactory::createTextField(
        fmt::format("{:.2f}", Settings::getWordsFrequency(false)),
        {0.01, 0.15},
        "Word Frequency",
        L"^[0-9]([.][0-9]{0,2})?$"
    );
    wordFrequencyField->onTextFieldUpdate = [wordFrequencyField]() -> void { Settings::setWordsFrequency(wordFrequencyField->getInputString()); };
    panelSettings->addElement(wordFrequencyField);

    auto wordSize = UIElementFactory::createTextField(
        fmt::format("{:.2f}", Settings::getWordsSize(false)),
        {0.01, 0.25},
        "Word Size",
        L"^[0-9]([.][0-9]{0,2})?$"
    );
    wordSize->onTextFieldUpdate = [wordSize]() -> void { Settings::setWordsSize(wordSize->getInputString()); };
    panelSettings->addElement(wordSize);

    auto wordHighlight = UIElementFactory::createMenuCheckbox(
        1.0,
        {0.01, 0.35},
        "Word Highlight",
        Settings::isWordsHighlightEnabled(false),
        [](bool value) -> void { Settings::setWordsHighlight(value); }
    );
    panelSettings->addElement(wordHighlight);

    auto UIScaleSetting = UIElementFactory::createTextField(
        fmt::format("{:.2f}", Settings::getUIScale(false)),
        {0.01, 0.70},
        "UI Scale (max 5.99)",
        L"^[0-2]([.][0-9]{0,2})?$"
    );
    UIScaleSetting->onTextFieldUpdate = [UIScaleSetting]() -> void { Settings::setUIScale(UIScaleSetting->getInputString()); };
    panelSettings->addElement(UIScaleSetting);

    auto wordFontComboBox = UIElementFactory::createComboBox(
        fmt::format("{}", Settings::getWordsFontName(false)),
        {0.01, 0.45},
        "Word Font"
    );
    wordFontComboBox->addRadioButton("voye", [wordFontComboBox]() -> void { wordFontComboBox->setChosenText("voye"); Settings::setWordsFontName("voye"); wordFontComboBox->deactivate(); });
    wordFontComboBox->addRadioButton("aleoRegular", [wordFontComboBox]() -> void { wordFontComboBox->setChosenText("aleoRegular"); Settings::setWordsFontName("aleoRegular"); wordFontComboBox->deactivate(); });
    wordFontComboBox->addRadioButton("arial", [wordFontComboBox]() -> void { wordFontComboBox->setChosenText("arial"); Settings::setWordsFontName("arial"); wordFontComboBox->deactivate(); });
    panelSettings->addElement(wordFontComboBox);

    panelSettings->addElement(UIElementFactory::createMenuButton("Back", []() -> void { GameInterface::setMenuState(MENU_DEFAULT); }, {0.1, 0.94}, {100, 40}));
    panelSettings->addElement(UIElementFactory::createMenuButton("Restore Default", []() -> void { Settings::restoreDefaultSettings(); }, {0.4, 0.94}, {200, 40}));
    panelSettings->addElement(UIElementFactory::createMenuButton("Save", []() -> void { Settings::saveSettingsPanel(); }, {0.65, 0.94}, {100, 40}));

    // ================= Setting up the STATE_NEWGAMESETUP Panel
    auto panelNewGameSetup = UIElementFactory::createPanel(panelWindow, {740, 600}, {0.5, 0.30}, PANEL_NEWGAMESETUP);

    auto gameCriteriasLabel = new TextLabel("Check the criteria for game end", {0.5, 0.03});
    panelNewGameSetup->addElement(gameCriteriasLabel);

    auto gameLocaleLabel = new TextLabel("Setup Locale", {0.5, 0.5});
    panelNewGameSetup->addElement(gameLocaleLabel);

    auto gameLocaleTextField = UIElementFactory::createTextField(
        fmt::format("{}", Settings::getWordLocale(false)),
        {0.2, 0.62},
        "Word Locale:",
        L".{0,17}"
    );
    gameLocaleTextField->setOffsetBodyAfterText(140);
    panelNewGameSetup->addElement(gameLocaleTextField);

    auto confirmLocaleButton = UIElementFactory::createMenuButton(
            "Confirm",
            []() -> void {},
            {0.75, 0.64},
            {100, 34}
    );
    panelNewGameSetup->addElement(confirmLocaleButton);

    gameLocaleTextField->onTextFieldUpdate = [confirmLocaleButton]() -> void { confirmLocaleButton->body.setFillColor(sf::Color::Red); };
    confirmLocaleButton->onClick = [confirmLocaleButton, gameLocaleTextField]() -> void { WordLocales::validateLocale(confirmLocaleButton, gameLocaleTextField); };

    auto localeExplanationLabel = new TextLabel(
            "Locales are taken from Resources/Locales/ and you can put your own there.\nYou must follow the format:\n"
            "cUsTomLocAle.txt -> cUsTomLocAle;\tspaceWords.txt -> spaceWords",
            {0.5, 0.7}
        );
    panelNewGameSetup->addElement(localeExplanationLabel);

    panelNewGameSetup->addElement(UIElementFactory::createMenuButton(
        "Start",
        []() -> void { GameStatistics::getInstance()->setupDefaultStatistics(); Game::setGameState(Game::STATE_PLAYING, true); },
        {0.60, 0.95},
        {100, 40}
    ));

    panelNewGameSetup->addElement(UIElementFactory::createMenuButton(
        "Back",
        []() -> void { GameStatistics::getInstance()->setupDefaultStatistics(); Game::setGameState(Game::STATE_MENU, true); },
        {0.40, 0.95},
        {100, 40}
    ));

    auto gameEndCriterium_wordsMissed = UIElementFactory::createMenuCheckbox(
        1.0,
        {0.01, 0.15},
        "End on Words Missed",
        Settings::getEndGameCriteriumBool("endGame_missedWords_bool"),
        [](bool value) -> void { Settings::setEndGameCriteriumBool("endGame_missedWords_bool", value); }
    );
    panelNewGameSetup->addElement(gameEndCriterium_wordsMissed);

    auto gameEnd_wordsMissedTextField = UIElementFactory::createTextField(
        fmt::format("{}", Settings::getEndGameCriterium_missedWords()),
        {0.5, 0.15},
        "Value: ",
        L"^[1-9]{1}[0-9]{0,9}$"
    );
    gameEnd_wordsMissedTextField->setOffsetBodyAfterText(90);
    gameEnd_wordsMissedTextField->onTextFieldUpdate = [gameEnd_wordsMissedTextField]() -> void {
        Settings::setEndGameCriterium_missedWords(gameEnd_wordsMissedTextField->getInputString());
    };
    panelNewGameSetup->addElement(gameEnd_wordsMissedTextField);

    auto gameEndCriterium_time = UIElementFactory::createMenuCheckbox(
        1.0,
        {0.01, 0.25},
        "End on Time",
        Settings::getEndGameCriteriumBool("endGame_time_bool"),
        [](bool value) -> void { Settings::setEndGameCriteriumBool("endGame_time_bool", value); }
    );
    panelNewGameSetup->addElement(gameEndCriterium_time);

    auto gameEndCriterium_timeTextField = UIElementFactory::createTextField(
        fmt::format("{:.2f}", Settings::getEndGameCriterium_time().count()),
        {0.5, 0.25},
        "Value: ",
        L"^[0-9]{1,7}([.][0-9]{0,5})?$"
    );
    gameEndCriterium_timeTextField->setOffsetBodyAfterText(90);
    gameEndCriterium_timeTextField->onTextFieldUpdate = [gameEndCriterium_timeTextField]() -> void {
        Settings::setEndGameCriterium_time(gameEndCriterium_timeTextField->getInputString());
    };
    panelNewGameSetup->addElement(gameEndCriterium_timeTextField);

    auto gameEndCriterium_score = UIElementFactory::createMenuCheckbox(
        1.0,
        {0.01, 0.35},
        "End on Score",
        Settings::getEndGameCriteriumBool("endGame_score_bool"),
        [](bool value) -> void { Settings::setEndGameCriteriumBool("endGame_score_bool", value); }
    );
    panelNewGameSetup->addElement(gameEndCriterium_score);

    auto gameEndCriterium_scoreTextField = UIElementFactory::createTextField(
        fmt::format("{}", Settings::getEndGameCriterium_score()),
        {0.5, 0.35},
        "Value: ",
        L"^[1-9]{1}[0-9]{0,9}$"
    );
    gameEndCriterium_scoreTextField->setOffsetBodyAfterText(90);
    gameEndCriterium_scoreTextField->onTextFieldUpdate = [gameEndCriterium_scoreTextField]() -> void {
        Settings::setEndGameCriterium_score(gameEndCriterium_scoreTextField->getInputString());
    };
    panelNewGameSetup->addElement(gameEndCriterium_scoreTextField);

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
        {0.5, 0.35},
        "",
        L".{0,35}", // Anything can be in there as its up to user what he types
        []() -> void { WordSpawner::getInstance()->manageWords(); }
    );
    gameInputWords->body.setSize({420, 30});
    gameInputWords->setState(FOCUSED_ALWAYS);
    panelGameStatistics->addElement(gameInputWords);

    auto averageTimePerWord = UIElementFactory::createStatisticsDynamicLabel(
        {0.79, 0.10},
        []() -> std::string { return "Av. word/s: " + GameStatistics::getInstance()->formatTime(GameStatistics::getInstance()->getAverageTimePerWord()); }
    );
    panelGameStatistics->addElement(averageTimePerWord);

    auto timePassed = UIElementFactory::createStatisticsDynamicLabel(
        {0.79, 0.25},
        []() -> std::string { return "Time passed (s): " + GameStatistics::getInstance()->formatTime(GameStatistics::getInstance()->getTimePassedSinceStart()); }
    );
    panelGameStatistics->addElement(timePassed);

    auto wordsScored = UIElementFactory::createStatisticsDynamicLabel(
        {0.05, 0.10},
        []() -> std::string { return "Words Scored: " + std::to_string(GameStatistics::getInstance()->getWordsScored()); }
    );
    panelGameStatistics->addElement(wordsScored);

    auto wordsMissed = UIElementFactory::createStatisticsDynamicLabel(
        {0.05, 0.25},
        []() -> std::string { return "Words Missed: " + std::to_string(GameStatistics::getInstance()->getWordsMissed()); }
    );
    panelGameStatistics->addElement(wordsMissed);

    auto generalScore = UIElementFactory::createStatisticsDynamicLabel(
        {0.05, 0.40},
        []() -> std::string { return "General Score: " + std::to_string(GameStatistics::getInstance()->getWordsGeneralScore()); }
    );
    panelGameStatistics->addElement(generalScore);

    // It cant be normal TextLabel and needs to be Dynamic as it needs updating in Settings::applySettings();
    auto whatSettingsLabel = UIElementFactory::createInfoDynamicLabel(
        {0.2, 0.85},
        []() -> std::string {
            return fmt::format(
            "[Selected Settings]\t Font:  {},\t Frequency:  {},\t Speed:  {},\t Size:  {},\t Highlight:  {},\t UIScale:  {},\t Locale:  {}",
                            Settings::getWordsFontName(false), Settings::getWordsFrequency(false), Settings::getWordsSpeed(false),
                            Settings::getWordsSize(false), Settings::isWordsHighlightEnabled(false), Settings::getUIScale(false),
                            Settings::getWordLocale(false)
            );
        }
    );
    panelGameStatistics->addElement(whatSettingsLabel);

    auto whatGameSettingsPanel = UIElementFactory::createInfoDynamicLabel(
        {0.2, 0.75},
        []() -> std::string {
            return fmt::format("[Selected End Criterium]\t {}", Settings::buildEndGameSettings());
        }
    );
    panelGameStatistics->addElement(whatGameSettingsPanel);

    // Game Over Panel
    auto panelGameOver = UIElementFactory::createPanel(panelWindow, {700, 560}, {0.5, 0.25}, PANEL_GAMEOVER);
    panelGameOver->body.setOutlineThickness(4);
    panelGameOver->body.setOutlineColor(sf::Color(209, 31, 34));
    panelGameOver->body.setFillColor(sf::Color(15, 15, 15, 210));

    auto gameOverLabel = new TextLabel("GAME OVER", {0.5, 0.01});
    gameOverLabel->getText().setCharacterSize(GameInterface::bigCharacterSize);
    panelGameOver->addElement(gameOverLabel);

    auto newHighscoreLabel = new TextLabel("", {0.5, 0.75});
    newHighscoreLabel->setAlignType(ALIGN_HALFSIZE);
    newHighscoreLabel->getText().setCharacterSize(GameInterface::mediumLiteCharacterSize);
    panelGameOver->addElement(newHighscoreLabel);

    auto onGameOverUpdateString = []() -> std::string {
        return fmt::format(
            "General Score:\t {}\nWords Scored:\t {}\nWords Missed:\t {}\nAv. Word/s:\t {}\nTime Passed (s):\t {}",
            std::to_string(GameStatistics::getInstance()->getWordsGeneralScore()),
            std::to_string(GameStatistics::getInstance()->getWordsScored()),
            std::to_string(GameStatistics::getInstance()->getWordsMissed()),
            GameStatistics::getInstance()->formatTime(GameStatistics::getInstance()->getAverageTimePerWord()),
            GameStatistics::getInstance()->formatTime(GameStatistics::getInstance()->getTimePassedSinceStart())
        );
    };
    auto gameOverOverallStats = new DynamicTextLabel({0.5, 0.22}, onGameOverUpdateString);
    gameOverOverallStats->getText().setCharacterSize(GameInterface::mediumLiteCharacterSize);
    panelGameOver->addElement(gameOverOverallStats);

    panelGameOver->addElement(UIElementFactory::createMenuButton("Back to Menu", []() -> void { Game::backToMenu(); }, {0.5, 0.90}, {150, 40}));
    // Setup Pause Panel
    auto panelPause = UIElementFactory::createPanel(panelWindow, {700, 560}, {0.5, 0.25}, PANEL_PAUSE);
    panelPause->body.setOutlineThickness(4);
    panelPause->body.setOutlineColor(sf::Color(209, 31, 34));
    panelPause->body.setFillColor(sf::Color(15, 15, 15, 210));

    auto pauseLabel = new TextLabel("PAUSE", {0.5, 0.01});
    pauseLabel->getText().setCharacterSize(GameInterface::bigCharacterSize);
    panelPause->addElement(pauseLabel);

    panelPause->addElement(UIElementFactory::createMenuButton("Back to Menu", []() -> void { Game::backToMenu(); }, {0.5, 0.90}, {150, 40}));

    // Adding each Panel to Panels Vector
    GameInterface::addPanelToVector(panelWindow);
    GameInterface::addPanelToVector(panelMenu);
    GameInterface::addPanelToVector(panelSettings);
    GameInterface::addPanelToVector(panelWords);
    GameInterface::addPanelToVector(panelGameStatistics);
    GameInterface::addPanelToVector(panelNewGameSetup);
    GameInterface::addPanelToVector(panelGameOver);
    GameInterface::addPanelToVector(panelPause);
    // Adds Highscores Panel to vector
    setupHighscoresPanel(panelWindow);

    GameInterface::updatePanels();
}

void GameInterface::setBackgroundSprite(sf::Sprite& sprite) {
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

Panel* GameInterface::getPanelByType(PanelType panelType) {
    for(auto panel : GameInterface::panels) {
        if(panel->getPanelType() == panelType) {
            return panel;
        }
    }

    return nullptr;
}

void setupHighscoresPanel(Panel* panelWindow) {
    auto panel = UIElementFactory::createPanel(panelWindow, {880, 800}, {0.5, 0.18}, PANEL_HIGHSCORES);
    auto MAXHIGHSCORES_AMOUNT = 5;

    for(auto i = 0; i < MAXHIGHSCORES_AMOUNT; i++) {
        // score,timepassed,wordFrequency,wordSpeed,chosenLanguage
        auto dynamicHighscoreLabel = UIElementFactory::createStatisticsDynamicLabel(
                {0.03, static_cast<float>(0.19 * i + 0.02)},
                [i]() -> std::string {
                    auto highscore = Highscores::getInstance()->getHighscore(i);
                    return fmt::format("TOP {}\t\t\tScore: {}\nTime Passed: {}\tW. Frequency: {}\tW. Speed: {}\nChosen Locale: {}",
                        i + 1, highscore[0], highscore[1], highscore[2], highscore[3], highscore[4]);
                }
        );
        dynamicHighscoreLabel->setAlignType(ALIGN_NONE);
        panel->addElement(dynamicHighscoreLabel);
    }
    panel->addElement(UIElementFactory::createMenuButton("Back", []() -> void { GameInterface::setMenuState(GameInterface::MENU_DEFAULT); }, {0.5, 0.96}, {140, 40}));

    GameInterface::addPanelToVector(panel);
}