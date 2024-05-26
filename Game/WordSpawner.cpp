#include "WordSpawner.h"

#include <random>

#include "Game.h"
#include "GameInterface.h"
#include "../Settings.h"
#include "../UI/word.h"
#include "fmt/ostream.h"

double chooseWordYRatio();

float getRandomFloat(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

void WordSpawner::spawnWord() {
    if(Game::getGameState() != Game::STATE_PLAYING) {
        return;
    }

    if(WordSpawner::canSpawnWord() == false) {
        return;
    }

    auto wordsPanel = GameInterface::getPanelByType(PANEL_WORDS);
    auto word = new Word();
    // Words are not "left upper corner", but they are always -width, so its their position is actually (rightCorner, upper)
    // Thats why we calculate x ratio here to make them properly aligned to left side of panel
    word->setPosRatios(word->getText().getLocalBounds().width / wordsPanel->body.getLocalBounds().width, chooseWordYRatio());

    wordsPanel->addElement(word);

    WordSpawner::lastSpawnTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

bool WordSpawner::canSpawnWord() {
    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    auto timeSinceLastSpawn = currentTime - lastSpawnTime;

    return timeSinceLastSpawn >= std::chrono::milliseconds(static_cast<int>(Settings::getWordsFrequency() * 1000));
}

void WordSpawner::moveWords() {
    auto wordsPanel = GameInterface::getPanelByType(PANEL_WORDS);
    if(wordsPanel == nullptr) {
        return;
    }

    for(auto word : wordsPanel->UIElements) {
        if(word->posXRatio >= 1.0) {
            auto it = std::find(wordsPanel->UIElements.begin(), wordsPanel->UIElements.end(), word);
            if (it != wordsPanel->UIElements.end()) {
                delete *it;
                wordsPanel->UIElements.erase(it);
            }
            continue;
        }

        word->setPosRatios(word->posXRatio + Settings::getWordsSpeed(), word->posYRatio);
        word->update();
    }
}

std::vector<double> lastYRatios = std::vector<double>{0.1, 0.65, 0.4, 0.9};
double chooseWordYRatio() {
    auto randomFloat = getRandomFloat(0, 0.95);

    for(auto value : lastYRatios) {
        if(std::abs(randomFloat - value) <= 0.08) {
            //fmt::println("Value that is bad: {} because vector has: {}", randomFloat, value);
            return chooseWordYRatio();
        }
    }

    //fmt::println("Value good {}", randomFloat);
    lastYRatios.erase(lastYRatios.begin());
    lastYRatios.push_back(randomFloat);
    return randomFloat;
}