#include "WordSpawner.h"

#include <random>

#include "Game.h"
#include "GameInterface.h"
#include "GameStatistics.h"
#include "../Settings.h"
#include "../UI/word.h"
#include "fmt/ostream.h"

WordSpawner* WordSpawner::wordSpawner_= nullptr;
// Holds last Pos Y Ratio values, so words cannot by chance appear on the same position after spawn
std::vector<double> lastYRatios;

double chooseWordYRatio();
void setAllWordsBodiesToZero(Panel* panel);

float getRandomFloat(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

WordSpawner* WordSpawner::getInstance() {
    if(wordSpawner_== nullptr){
        wordSpawner_ = new WordSpawner();
        wordSpawner_->lastSpawnTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        // Initialized with random values, all that matters is that they are different
        lastYRatios = std::vector<double>{0.1, 0.65, 0.4, 0.9};
    }
    return wordSpawner_;
}

// If canSpawnWord() is true then it creates
// new word at X: left side of panel, Y: randomly chosen
// The text of Word is set in Word's constructor
void WordSpawner::spawnWord() {
    if(Game::getInstance()->getGameState() != STATE_PLAYING) {
        return;
    }

    if(this->canSpawnWord() == false) {
        return;
    }

    auto wordsPanel = GameInterface::getInstance()->getPanelByType(PANEL_WORDS);
    auto word = new Word();
    // Words are not "left upper corner", but they are always -width, so its their position is actually (rightCorner, upper)
    // ^ The reason words are like that is to they instantly disappear when they touch the right side.
    // Thats why we calculate x ratio here to make them properly aligned to left side of panel
    word->posRatio.setValues(word->getText().getLocalBounds().width / wordsPanel->body.getLocalBounds().width, chooseWordYRatio());

    wordsPanel->addElement(word);

    this->lastSpawnTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

// Spawn word method used when loading the game state from a save file
void WordSpawner::spawnWord(float ratioX, float ratioY, std::string text) {
    auto wordsPanel = GameInterface::getInstance()->getPanelByType(PANEL_WORDS);
    auto word = new Word();

    sf::String sfString = sf::String::fromUtf8(text.begin(), text.end());
    word->getText().setString(sfString);
    // Words are not "left upper corner", but they are always -width, so its their position is actually (rightCorner, upper)
    // Thats why we calculate x ratio here to make them properly aligned to left side of panel
    word->posRatio.setValues(ratioX, ratioY);

    wordsPanel->addElement(word);

    this->lastSpawnTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

// Checks if enough time has passed (comparess timeSinceLastSpawn with getWordsFrequency())
// Returns true if word can be spawned
bool WordSpawner::canSpawnWord() {
    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    auto timeSinceLastSpawn = currentTime - lastSpawnTime;

    return timeSinceLastSpawn >= std::chrono::milliseconds(static_cast<int>(Settings::getInstance()->getWordsFrequency(false) * 1000));
}

// Moves all words by getWordsSpeed(false) value.
// If words are touching the right side (their pos X ratio is at 1.0) then they are deleted.
// In case of word removal statistics are updated (words missed are increased)
void WordSpawner::moveWords() {
    auto wordsPanel = GameInterface::getInstance()->getPanelByType(PANEL_WORDS);
    if(wordsPanel == nullptr) {
        return;
    }

    // Updates dynamic label containing info about Time that has passed since start of the game.
    GameStatistics::getInstance()->updateTimePassedSinceStart();

    for(auto word : wordsPanel->UIElements) {
        if(word->posRatio.getX() >= 1.0) {
            wordsPanel->removeElement(word);
            GameStatistics::getInstance()->increaseWordsMissed(1);
            GameStatistics::getInstance()->updateStatistics(GameInterface::getInstance()->getPanelByType(PANEL_GAMESTATISTICS));
            continue;
        }

        word->posRatio.setValues(word->posRatio.getX() + Settings::getInstance()->getWordsSpeed(false), word->posRatio.getY());
        word->update();
    }
}

// Takes input from text field in PANEL_GAMESTATISTICS and compares it with all words
// to set their highlight width.
// In case an input found a word that matches it 1:1, then the input is cleared, the word is removed and
// the statistics are updated (like score and words scored)
void WordSpawner::manageWords() {
    auto panelGameStatistics = GameInterface::getInstance()->getPanelByType(PANEL_GAMESTATISTICS);
    if(panelGameStatistics == nullptr) {
        throw std::runtime_error("WordSpawner::manageWords() can't seem to find PANEL_GAMESTATISTICS. Does it exist? (Check GameInterface::getInstance()->setupPanels())");
    }

    TextField* textField = nullptr;
    for(auto uielement : panelGameStatistics->UIElements) {
        if(uielement->getType() == TEXTFIELD) {
            textField = static_cast<TextField*>(uielement);
            break;
        }
    }

    if(textField == nullptr) {
        throw std::runtime_error("WordSpawner::manageWords() can't seem to find TextField in PANEL_GAMESTATISTICS. Does it exist? (Check GameInterface::getInstance()->setupPanels())");
    }

    auto panelWords = GameInterface::getInstance()->getPanelByType(PANEL_WORDS);
    if(panelWords == nullptr) {
        throw std::runtime_error("WordSpawner::manageWords() can't seem to find PANEL_WORDS. Does it exist? (Check GameInterface::getInstance()->setupPanels())");
    }

    // sf::String converted to utf32 string as we later in code below use e.g.: sf::Uint32 letter = wordString[index];
    auto input = textField->getInputString().toUtf32();

    for(int uiIndex = 0; uiIndex < panelWords->UIElements.size(); uiIndex++) {
        auto word = static_cast<Word*>(panelWords->UIElements[uiIndex]);
        if (word) { // true if isn't a nullptr (was sucessfully casted)
            auto bodyHeight = word->body.getSize().y;
            if(input.size() == 0) {
                word->body.setSize({0,bodyHeight});
                continue;
            }

            auto inputMatchesWord = false; // Dynamically changing boolean variable that will check if input equals word
            auto finalBodyWidth = 0.0f;

            auto wordString = word->getText().getString();
            auto wordSize = wordString.getSize();
            // We cant use .getCharacterSize() as it will not reflect the real size of single letter
            // getGlobalBounds MUST NOT be used in charSize. It's because we are scalling text in update() so it would collide
            auto charSize = (word->getText().getLocalBounds().width/wordSize);

            // wordString.getSize() returns std::size_t so we dont use int for index
            for (std::size_t index = 0; index < wordSize; index++) {
                if(!input[index]) {
                    inputMatchesWord = false;
                    break;
                }

                sf::Uint32 letter = wordString[index];
                if(letter == input[index]) {
                    inputMatchesWord = true;
                    finalBodyWidth += charSize;
                } else {
                    inputMatchesWord = false;
                    break;
                }
            }

            // Word is completed
            if(inputMatchesWord == true) {
                // DEBUG PRINT: (fmt doesnt by default support strings other than that one)
                // fmt::println("Completed Word: {}", wordString.toAnsiString());
                panelWords->removeElement(word);
                GameStatistics::getInstance()->increaseWordsScored(1);
                GameStatistics::getInstance()->increaseWordsGeneralScore(wordSize);
                textField->setInput("");
                textField->update();
                // We reset input so we must reset existing words' bodies as well
                setAllWordsBodiesToZero(panelWords);
                GameStatistics::getInstance()->updateStatistics(panelGameStatistics);
                return;
            }

            word->body.setSize({finalBodyWidth, bodyHeight});
            word->update();
        }
    }

}

// Useful method when you want to clear e.g. input or when the game is over and you want to remove all the words
void WordSpawner::clearWords(bool removeWords, bool removeHighlight, bool clearGameTextField) {
    if(removeWords || removeHighlight) {
        auto panelWords = GameInterface::getInstance()->getPanelByType(PANEL_WORDS);
        if (panelWords == nullptr) {
            throw std::runtime_error(
                    "WordSpawner::clearWords() can't seem to find PANEL_WORDS. Does it exist? (Check GameInterface::getInstance()->setupPanels())");
        }

        if(removeWords) {
            for(auto word : panelWords->UIElements) {
                delete word;
            }
            panelWords->UIElements.clear();
        } else {
            for(int uiIndex = 0; uiIndex < panelWords->UIElements.size(); uiIndex++) {
                auto word = static_cast<Word *>(panelWords->UIElements[uiIndex]);
                if (word) { // true if isn't a nullptr (was sucessfully casted)
                    auto bodyHeight = word->body.getSize().y;
                    word->body.setSize({0, bodyHeight});
                }
            }
        }
    }

    if(clearGameTextField) {
        auto panelGameStatistics = GameInterface::getInstance()->getPanelByType(PANEL_GAMESTATISTICS);
        if(panelGameStatistics == nullptr) {
            throw std::runtime_error("WordSpawner::clearWords() can't seem to find PANEL_GAMESTATISTICS. Does it exist? (Check GameInterface::getInstance()->setupPanels())");
        }

        for(auto uielement : panelGameStatistics->UIElements) {
            if(uielement->getType() == TEXTFIELD) {
                TextField* textField = nullptr;
                textField = static_cast<TextField*>(uielement);
                if(textField != nullptr) {
                    textField->setInput("");
                    textField->update();
                }
                break;
            }
        }
    }
}

// There is lastYRatios vector in WordSpawner.cpp that holds last four Y spawn positions of word
// When calculating Y ratio for new word we check if it is in last four positions chosen (there is a small range included too)
// Case: It is -> we are trying to find Y ratio again.
// Case: It isn't -> all is good, we push it to vector and return Y ratio.
double chooseWordYRatio() {
    auto randomFloat = getRandomFloat(0, 0.95);

    for(auto value : lastYRatios) {
        if(std::abs(randomFloat - value) <= 0.08) { // A 0.08 range, so words have some space beetween them if they spawn close
            //fmt::println("Value that is bad: {} because vector has: {}", randomFloat, value);
            return chooseWordYRatio();
        }
    }

    //fmt::println("Value good {}", randomFloat);
    lastYRatios.erase(lastYRatios.begin());
    lastYRatios.push_back(randomFloat);
    return randomFloat;
}

// Sets highlight of all bodies to 0 (meaning highlight won't be visible)
void setAllWordsBodiesToZero(Panel* panel) {
    for(auto word : panel->UIElements) {
        word->body.setSize({0, word->body.getSize().y});
    }
}