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
#include "../ResourceManagers/GameSaveManager.h"
#include "../Translator/WordLocales.h"

#include "fmt/chrono.h"

void setupHighscoresPanel(Panel* panelWindow);

GameInterface* GameInterface::gameInterface_ = nullptr;

GameInterface::GameInterface() {
    menuState = MENU_DEFAULT;
    backgroundSprite = sf::Sprite();
    backgroundTexture = sf::Texture();
    gameTitle = sf::Text();

    panels = std::vector<Panel*>();
}

GameInterface* GameInterface::getInstance() {
    if(gameInterface_ == nullptr){
        gameInterface_ = new GameInterface();
    }
    return gameInterface_;
}

void GameInterface::drawMenu() {
    drawMenuBackground();
    drawPanels();
}

void GameInterface::drawMenuBackground() {
    window.draw(getBackgroundSprite());
    window.draw(getGameTitle());
}

void GameInterface::drawPanels() {
    for (auto panel : panels) {
        if(panel->isVisible())
            panel->draw();
    }
}

void GameInterface::hideAllPanels() {
    for (auto panel : panels) {
        panel->setVisibility(false);
    }
}

void GameInterface::setMenuState(MenuStates state) {
    // We wont make a check for getState != state here as its called in Game:setGameState and we need it to not return prematurely.

    // Hide all panels and in switch we will show whatever panels should be visible
    hideAllPanels();

    switch(state) {
        case MENU_DEFAULT:
        {
            auto panelToShow = getPanelByType(PANEL_MENU);
            if(panelToShow != nullptr)
                panelToShow->setVisibility(true);
            menuState = state;
            break;
        }
        case MENU_SETTINGS:
        {
            auto panelToShow = getPanelByType(PANEL_SETTINGS);
            if(panelToShow != nullptr)
                panelToShow->setVisibility(true);
            menuState = state;
            break;
        }
        case MENU_LOAD:
        {
            auto panelToShow = getPanelByType(PANEL_LOADGAME);
            if(panelToShow != nullptr)
                panelToShow->setVisibility(true);
            menuState = state;
            break;
        }
        case MENU_HIGHSCORES:
        {
            auto panelToShow = getPanelByType(PANEL_HIGHSCORES);
            if(panelToShow != nullptr)
                panelToShow->setVisibility(true);
            menuState = state;
            break;
        }
        default:
            throw std::runtime_error("An unknown MenuState failed to be set: " + state);
    }
}

GameInterface::MenuStates GameInterface::getMenuState() {
    return menuState;
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
    backgroundTexture = BG_RenderTexture.getTexture();

    backgroundSprite.setTexture(backgroundTexture);
}

void GameInterface::setupGameTitle() {
    auto text = sf::Text();
    text.setFont(FontManager::Fonts["voye"]);
    text.setCharacterSize(GameInterface::hugeCharacterSize * Settings::getInstance()->getUIScale(false));
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
    text.setString("TYPETHRILL");
    setGameTitle(text);

    // Sets position of GameTitle
    updateGameTitle();
}

void GameInterface::updateGameTitle() {
    sf::Text text = getGameTitle();

    auto parentScale = panels[0]->body.getScale();
    text.setScale(parentScale);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width/2, textBounds.top + textBounds.height/2);

    sf::Vector2f textCenter(
        (window.getSize().x * 0.5),
        (window.getSize().y * 0.1)
    );

    text.setPosition(textCenter);

    setGameTitle(text);
}

void GameInterface::setupPanels() {
    // ================= Setting up the Window Panel
    auto panelWindow = new Panel({(float)originalWindowSize.x, (float)originalWindowSize.y}, {0,0});
    panelWindow->setPanelType(PANEL_WINDOW);

    // ================= Setting up the Menu Panel
    auto panelMenu = UIElementFactory::createPanel(panelWindow, {300, 440}, {0.5, 0.45}, PANEL_MENU);
    panelMenu->setVisibility(true);

    panelMenu->addElement(UIElementFactory::createMenuButton("New Game", []() -> void { Game::getInstance()->setGameState(STATE_NEWGAMESETUP, true); }, {0.5, 0.10}));
    panelMenu->addElement(UIElementFactory::createMenuButton("Load Game", []() -> void { GameInterface::getInstance()->setMenuState(MENU_LOAD); }, {0.5, 0.10}));
    panelMenu->addElement(UIElementFactory::createMenuButton("Settings", []() -> void { GameInterface::getInstance()->setMenuState(MENU_SETTINGS); }, {0.5, 0.10}));
    panelMenu->addElement(UIElementFactory::createMenuButton("Highscores", []() -> void { GameInterface::getInstance()->setMenuState(MENU_HIGHSCORES); }, {0.5, 0.10}));
    panelMenu->addElement(UIElementFactory::createMenuButton("Exit", []() -> void { exit(0); }, {0.5, 0.10}));
    int i = 0;
    for(auto uielement : panelMenu->UIElements) {
        uielement->posRatio.setValues(0.5, (uielement->posRatio.getY() + i * 0.19) + 0.02);
        i++;
    }

    // ================= Setting up the Settings Panel
    auto panelSettings = UIElementFactory::createPanel(panelWindow, {740, 740}, {0.5, 0.20}, PANEL_SETTINGS);

    auto settingsTextLabel = new TextLabel("Click 'Save' to keep changes even after game is closed.", {0.5, 0.84});
    panelSettings->addElement(settingsTextLabel);

    auto wordSpeedField = UIElementFactory::createTextField(
        fmt::format("{:.5f}", Settings::getInstance()->getWordsSpeed(false)),
        {0.01, 0.05},
        "Word Speed",
        L"^[0-9]([.][0-9]{0,5})?$" // L casts to wchar as we need wstring in wregex
    );
    wordSpeedField->onTextFieldUpdate = [wordSpeedField]() -> void { Settings::getInstance()->setWordsSpeed(wordSpeedField->getInputString()); };
    panelSettings->addElement(wordSpeedField);

    auto wordFrequencyField = UIElementFactory::createTextField(
        fmt::format("{:.2f}", Settings::getInstance()->getWordsFrequency(false)),
        {0.01, 0.15},
        "Word Frequency",
        L"^[0-9]([.][0-9]{0,2})?$"
    );
    wordFrequencyField->onTextFieldUpdate = [wordFrequencyField]() -> void { Settings::getInstance()->setWordsFrequency(wordFrequencyField->getInputString()); };
    panelSettings->addElement(wordFrequencyField);

    auto wordSize = UIElementFactory::createTextField(
        fmt::format("{:.2f}", Settings::getInstance()->getWordsSize(false)),
        {0.01, 0.25},
        "Word Size",
        L"^[0-9]([.][0-9]{0,2})?$"
    );
    wordSize->onTextFieldUpdate = [wordSize]() -> void { Settings::getInstance()->setWordsSize(wordSize->getInputString()); };
    panelSettings->addElement(wordSize);

    auto wordHighlight = UIElementFactory::createMenuCheckbox(
        1.0,
        {0.01, 0.35},
        "Word Highlight",
        Settings::getInstance()->isWordsHighlightEnabled(false),
        [](bool value) -> void { Settings::getInstance()->setWordsHighlight(value); }
    );
    panelSettings->addElement(wordHighlight);

    auto gameLocaleTextField = UIElementFactory::createTextField(
        fmt::format("{}", Settings::getInstance()->getWordLocale(false)),
        {0.01, 0.45},
        "Word Locale:",
        L".{0,17}"
    );
    panelSettings->addElement(gameLocaleTextField);

    auto confirmLocaleButton = UIElementFactory::createMenuButton(
            "Confirm",
            []() -> void {},
            {0.75, 0.47},
            {100, 34}
    );
    panelSettings->addElement(confirmLocaleButton);

    gameLocaleTextField->onTextFieldUpdate = [confirmLocaleButton]() -> void { confirmLocaleButton->body.setFillColor(sf::Color::Red); };
    confirmLocaleButton->onClick = [confirmLocaleButton, gameLocaleTextField]() -> void { WordLocales::validateLocale(confirmLocaleButton, gameLocaleTextField); };

    auto localeExplanationLabel = new TextLabel(
            "Locales are taken from Resources/Locales/ and you can put your own there.\nYou must follow the format:\n"
            "cUsTomLocAle.txt -> cUsTomLocAle;\tspaceWords.txt -> spaceWords",
            {0.5, 0.50}
        );
    panelSettings->addElement(localeExplanationLabel);

    auto wordFontComboBox = UIElementFactory::createComboBox(
        fmt::format("{}", Settings::getInstance()->getWordsFontName(false)),
        {0.01, 0.65},
        "Word Font"
    );
    wordFontComboBox->addComboButton("voye", [wordFontComboBox]() -> void { wordFontComboBox->setChosenText("voye"); Settings::getInstance()->setWordsFontName("voye"); wordFontComboBox->deactivate(); });
    wordFontComboBox->addComboButton("aleoRegular", [wordFontComboBox]() -> void { wordFontComboBox->setChosenText("aleoRegular"); Settings::getInstance()->setWordsFontName("aleoRegular"); wordFontComboBox->deactivate(); });
    wordFontComboBox->addComboButton("arial", [wordFontComboBox]() -> void { wordFontComboBox->setChosenText("arial"); Settings::getInstance()->setWordsFontName("arial"); wordFontComboBox->deactivate(); });
    panelSettings->addElement(wordFontComboBox);

    auto UIScaleSetting = UIElementFactory::createTextField(
        fmt::format("{:.2f}", Settings::getInstance()->getUIScale(false)),
        {0.01, 0.75},
        "UI Scale (max 5.99)",
        L"^[0-2]([.][0-9]{0,2})?$"
    );
    UIScaleSetting->onTextFieldUpdate = [UIScaleSetting]() -> void { Settings::getInstance()->setUIScale(UIScaleSetting->getInputString()); };
    panelSettings->addElement(UIScaleSetting);

    // Make UIScale hidden when ComboBox is active (otherwise they would overlap)
    wordFontComboBox->addElementToHide(UIScaleSetting);

    panelSettings->addElement(UIElementFactory::createMenuButton("Back", []() -> void { GameInterface::getInstance()->setMenuState(MENU_DEFAULT); }, {0.1, 0.94}, {100, 40}));
    panelSettings->addElement(UIElementFactory::createMenuButton("Restore Default", []() -> void { Settings::getInstance()->restoreDefaultSettings(); }, {0.4, 0.94}, {200, 40}));
    panelSettings->addElement(UIElementFactory::createMenuButton("Save", []() -> void { Settings::getInstance()->saveSettingsPanel(); }, {0.65, 0.94}, {100, 40}));

    // ================= Setting up the STATE_NEWGAMESETUP Panel
    auto panelNewGameSetup = UIElementFactory::createPanel(panelWindow, {740, 440}, {0.5, 0.30}, PANEL_NEWGAMESETUP);

    auto gameCriteriasLabel = new TextLabel("Check the criteria for game end", {0.5, 0.03});
    panelNewGameSetup->addElement(gameCriteriasLabel);

    panelNewGameSetup->addElement(UIElementFactory::createMenuButton(
        "Start",
        []() -> void { Game::getInstance()->onGameStart(); },
        {0.60, 0.90},
        {100, 40}
    ));

    panelNewGameSetup->addElement(UIElementFactory::createMenuButton(
        "Back",
        []() -> void { Game::getInstance()->setGameState(STATE_MENU, true); },
        {0.40, 0.90},
        {100, 40}
    ));

    auto gameEndCriterium_wordsMissed = UIElementFactory::createMenuCheckbox(
        1.0,
        {0.01, 0.15},
        "End on Words Missed",
        Settings::getInstance()->getEndGameCriteriumBool("endGame_missedWords_bool"),
        [](bool value) -> void { Settings::getInstance()->setEndGameCriteriumBool("endGame_missedWords_bool", value); }
    );
    panelNewGameSetup->addElement(gameEndCriterium_wordsMissed);

    auto gameEnd_wordsMissedTextField = UIElementFactory::createTextField(
        fmt::format("{}", Settings::getInstance()->getEndGameCriterium_missedWords()),
        {0.5, 0.15},
        "Value: ",
        L"^[1-9]{1}[0-9]{0,9}$"
    );
    gameEnd_wordsMissedTextField->setOffsetBodyAfterText(90);
    gameEnd_wordsMissedTextField->onTextFieldUpdate = [gameEnd_wordsMissedTextField]() -> void {
        Settings::getInstance()->setEndGameCriterium_missedWords(gameEnd_wordsMissedTextField->getInputString());
    };
    panelNewGameSetup->addElement(gameEnd_wordsMissedTextField);

    auto gameEndCriterium_time = UIElementFactory::createMenuCheckbox(
        1.0,
        {0.01, 0.30},
        "End on Time",
        Settings::getInstance()->getEndGameCriteriumBool("endGame_time_bool"),
        [](bool value) -> void { Settings::getInstance()->setEndGameCriteriumBool("endGame_time_bool", value); }
    );
    panelNewGameSetup->addElement(gameEndCriterium_time);

    auto gameEndCriterium_timeTextField = UIElementFactory::createTextField(
        fmt::format("{:.2f}", Settings::getInstance()->getEndGameCriterium_time().count()),
        {0.5, 0.30},
        "Value: ",
        L"^[0-9]{1,7}([.][0-9]{0,5})?$"
    );
    gameEndCriterium_timeTextField->setOffsetBodyAfterText(90);
    gameEndCriterium_timeTextField->onTextFieldUpdate = [gameEndCriterium_timeTextField]() -> void {
        Settings::getInstance()->setEndGameCriterium_time(gameEndCriterium_timeTextField->getInputString());
    };
    panelNewGameSetup->addElement(gameEndCriterium_timeTextField);

    auto gameEndCriterium_score = UIElementFactory::createMenuCheckbox(
        1.0,
        {0.01, 0.45},
        "End on Score",
        Settings::getInstance()->getEndGameCriteriumBool("endGame_score_bool"),
        [](bool value) -> void { Settings::getInstance()->setEndGameCriteriumBool("endGame_score_bool", value); }
    );
    panelNewGameSetup->addElement(gameEndCriterium_score);

    auto gameEndCriterium_scoreTextField = UIElementFactory::createTextField(
        fmt::format("{}", Settings::getInstance()->getEndGameCriterium_score()),
        {0.5, 0.45},
        "Value: ",
        L"^[1-9]{1}[0-9]{0,9}$"
    );
    gameEndCriterium_scoreTextField->setOffsetBodyAfterText(90);
    gameEndCriterium_scoreTextField->onTextFieldUpdate = [gameEndCriterium_scoreTextField]() -> void {
        Settings::getInstance()->setEndGameCriterium_score(gameEndCriterium_scoreTextField->getInputString());
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

    // It cant be normal TextLabel and needs to be Dynamic as it needs updating in Settings::getInstance()->applySettings();
    auto whatSettingsLabel = UIElementFactory::createInfoDynamicLabel(
        {0.2, 0.85},
        []() -> std::string {
            return fmt::format(
            "[Selected Settings]\t Font:  {},\t Frequency:  {},\t Speed:  {},\t Size:  {},\t Highlight:  {},\t UIScale:  {},\t Locale:  {}",
                            Settings::getInstance()->getWordsFontName(false), Settings::getInstance()->getWordsFrequency(false), Settings::getInstance()->getWordsSpeed(false),
                            Settings::getInstance()->getWordsSize(false), Settings::getInstance()->isWordsHighlightEnabled(false), Settings::getInstance()->getUIScale(false),
                            Settings::getInstance()->getWordLocale(false)
            );
        }
    );
    panelGameStatistics->addElement(whatSettingsLabel);

    auto whatGameSettingsPanel = UIElementFactory::createInfoDynamicLabel(
        {0.2, 0.75},
        []() -> std::string {
            return fmt::format("[Selected End Criterium]\t {}", Settings::getInstance()->buildEndGameSettings());
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

    panelGameOver->addElement(UIElementFactory::createMenuButton("Back to Menu", []() -> void { Game::getInstance()->backToMenu(); }, {0.5, 0.90}, {150, 40}));
    // Setup Pause Panel
    auto panelPause = UIElementFactory::createPanel(panelWindow, {700, 560}, {0.5, 0.25}, PANEL_PAUSE);
    panelPause->body.setOutlineThickness(4);
    panelPause->body.setOutlineColor(sf::Color(209, 31, 34));
    panelPause->body.setFillColor(sf::Color(15, 15, 15, 210));

    auto pauseLabel = new TextLabel("PAUSE", {0.5, 0.01});
    pauseLabel->getText().setCharacterSize(GameInterface::bigCharacterSize);
    panelPause->addElement(pauseLabel);

    auto buttonSave1 = UIElementFactory::createSaveSlot("Save Slot", {0.5, 0.25}, {640, 120}, 1 /* save slot index */);
    panelPause->addElement(buttonSave1);
    // Dynamic text label that will show description of save/load file
    auto saveDescriptionLabel1 = UIElementFactory::createSaveSlotDynamicLabel(
        {0.01, 0.01}, 1 /* save slot index */, panelPause, buttonSave1
    );

    auto buttonSave2 = UIElementFactory::createSaveSlot("Save Slot", {0.5, 0.50}, {640, 120}, 2 /* save slot index */);
    panelPause->addElement(buttonSave2);
    // Dynamic text label that will show description of save/load file
    auto saveDescriptionLabel2 = UIElementFactory::createSaveSlotDynamicLabel(
        {0.01, 0.01}, 2 /* save slot index */, panelPause, buttonSave2
    );

    auto buttonSave3 = UIElementFactory::createSaveSlot("Save Slot", {0.5, 0.75}, {640, 120}, 3 /* save slot index */);
    panelPause->addElement(buttonSave3);
    // Dynamic text label that will show description of save/load file
    auto saveDescriptionLabel3 = UIElementFactory::createSaveSlotDynamicLabel(
        {0.01, 0.01}, 3 /* save slot index */, panelPause, buttonSave3
    );

    std::function<void()> lambdaShowSaveSlots = [buttonSave1, buttonSave2, buttonSave3, saveDescriptionLabel1, saveDescriptionLabel2, saveDescriptionLabel3]() -> void
    {
        buttonSave1->toggleVisibility(); buttonSave2->toggleVisibility(); buttonSave3->toggleVisibility();
        saveDescriptionLabel1->toggleVisibility(), saveDescriptionLabel2->toggleVisibility(), saveDescriptionLabel3->toggleVisibility();
    };

    panelPause->addElement(UIElementFactory::createMenuButton("Back to Menu", [lambdaShowSaveSlots]() -> void { lambdaShowSaveSlots(); Game::getInstance()->backToMenu(); }, {0.85, 0.94}, {150, 40}));
    panelPause->addElement(UIElementFactory::createMenuButton(
        "Show Save Slots",
        [buttonSave1, buttonSave2, buttonSave3, saveDescriptionLabel1, saveDescriptionLabel2, saveDescriptionLabel3]() -> void
        {
            buttonSave1->toggleVisibility(); buttonSave2->toggleVisibility(); buttonSave3->toggleVisibility();
            saveDescriptionLabel1->toggleVisibility(), saveDescriptionLabel2->toggleVisibility(), saveDescriptionLabel3->toggleVisibility();
        },
        {0.15, 0.94}, {150, 40})
    );
    panelPause->addElement(UIElementFactory::createMenuButton("Resume", []() -> void { Game::getInstance()->pause(); }, {0.5, 0.94}, {150, 40}));

    // Load Game Panel Setup
    auto panelLoadGame = UIElementFactory::createPanel(panelWindow, {700, 660}, {0.5, 0.25}, PANEL_LOADGAME);

    auto loadGameLabel = new TextLabel("LOAD GAME", {0.5, 0.01});
    loadGameLabel->getText().setCharacterSize(GameInterface::bigCharacterSize);
    panelLoadGame->addElement(loadGameLabel);

    panelLoadGame->addElement(UIElementFactory::createMenuButton("Back", []() -> void { GameInterface::getInstance()->setMenuState(MENU_DEFAULT); }, {0.5, 0.94}, {100, 40}));

    auto buttonLoad1 = UIElementFactory::createMenuButton("Load Slot", []() -> void { GameSaveManager::getInstance()->loadGame(1); }, {0.5, 0.25}, {600, 120});
    panelLoadGame->addElement(buttonLoad1);
    auto buttonLoadDescriptionLabel1 = UIElementFactory::createInfoDynamicLabel(
        {0.01, 0.01},
        []() -> std::string {
            return GameSaveManager::getInstance()->getSlotDescription(1);
        }
    );
    panelLoadGame->addElement(buttonLoadDescriptionLabel1);
    buttonLoadDescriptionLabel1->setParent(buttonLoad1);

    auto buttonLoad2 = UIElementFactory::createMenuButton("Load Slot", []() -> void { GameSaveManager::getInstance()->loadGame(2); }, {0.5, 0.50}, {600, 120});
    panelLoadGame->addElement(buttonLoad2);
    auto buttonLoadDescriptionLabel2 = UIElementFactory::createInfoDynamicLabel(
        {0.01, 0.01},
        []() -> std::string {
            return GameSaveManager::getInstance()->getSlotDescription(2);
        }
    );
    panelLoadGame->addElement(buttonLoadDescriptionLabel2);
    buttonLoadDescriptionLabel2->setParent(buttonLoad2);

    auto buttonLoad3 = UIElementFactory::createMenuButton("Load Slot", []() -> void { GameSaveManager::getInstance()->loadGame(3); }, {0.5, 0.75}, {600, 120});
    panelLoadGame->addElement(buttonLoad3);
    auto buttonLoadDescriptionLabel3 = UIElementFactory::createInfoDynamicLabel(
        {0.01, 0.01},
        []() -> std::string {
            return GameSaveManager::getInstance()->getSlotDescription(3);
        }
    );
    panelLoadGame->addElement(buttonLoadDescriptionLabel3);
    buttonLoadDescriptionLabel3->setParent(buttonLoad3);

    // Adding each Panel to Panels Vector
    GameInterface::addPanelToVector(panelWindow);
    GameInterface::addPanelToVector(panelMenu);
    GameInterface::addPanelToVector(panelSettings);
    GameInterface::addPanelToVector(panelWords);
    GameInterface::addPanelToVector(panelGameStatistics);
    GameInterface::addPanelToVector(panelNewGameSetup);
    GameInterface::addPanelToVector(panelGameOver);
    GameInterface::addPanelToVector(panelPause);
    GameInterface::addPanelToVector(panelLoadGame);
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

    panel->addElement(UIElementFactory::createMenuButton("Back", []() -> void { GameInterface::getInstance()->setMenuState(GameInterface::MENU_DEFAULT); }, {0.5, 0.96}, {140, 40}));

    GameInterface::getInstance()->addPanelToVector(panel);
}