#include "Word.h"

#include <random>

#include "../Globals.h"
#include "../Game/GameInterface.h"
#include "../ResourceManagers/FontManager.h"
#include "../Settings.h"
#include "../Translator/WordLanguages.h"

Word::Word() {
    auto sfText = sf::Text();
    sfText.setFont(FontManager::Fonts[Settings::getWordsFontName()]);
    sfText.setCharacterSize(GameInterface::smallCharacterSize * Settings::getWordsSize());
    sfText.setFillColor(sf::Color::White);
    sfText.setOutlineColor(sf::Color::Black);
    sfText.setOutlineThickness(1);

    auto randomWord = WordLanguages::getRandomWord();
    sf::String sfString(randomWord);
    sfText.setString(sfString);

    this->setText(sfText);

    this->visibility = true;
}

void Word::draw() {
    if(text.getString().isEmpty() == false) {
        window.draw(this->text);
    }
}

void Word::update() {
    sf::Vector2f parentScale = parent->body.getScale();

    this->text.setScale(parentScale);

    auto textBounds = this->text.getGlobalBounds();

    sf::Vector2f newPosition(
        parent->body.getPosition().x + this->posXRatio * parent->body.getSize().x * parentScale.x - textBounds.width,
        parent->body.getPosition().y + this->posYRatio * parent->body.getSize().y * parentScale.y
    );
    this->text.setPosition(newPosition);
}

sf::Text& Word::getText() {
    return this->text;
}

void Word::setText(sf::Text& text) {
    this->text = text;
}

void Word::move(float x, float y) {
    this->text.move(x, y);
}

void Word::setPosition(float x, float y) {
    this->text.setPosition(x, y);
}