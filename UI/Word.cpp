#include "Word.h"

#include <random>

#include "../Globals.h"
#include "../Game/GameInterface.h"
#include "../ResourceManagers/FontManager.h"
#include "../Settings.h"
#include "../Translator/WordLanguages.h"

Word::Word() {
    this->setType(WORD);

    this->body =sf::RectangleShape({0,static_cast<float>(GameInterface::smallCharacterSize * Settings::getWordsSize(false))});
    this->body.setFillColor(sf::Color::Yellow);

    auto sfText = sf::Text();
    sfText.setFont(FontManager::Fonts[Settings::getWordsFontName(false)]);
    sfText.setCharacterSize(GameInterface::smallCharacterSize * Settings::getWordsSize(false));
    sfText.setFillColor(sf::Color::White);
    sfText.setOutlineColor(sf::Color::Black);
    sfText.setOutlineThickness(1);

    auto randomWord = WordLanguages::getRandomWord();
    sf::String sfString = sf::String::fromUtf8(randomWord.begin(), randomWord.end());
    sfText.setString(sfString);

    this->setText(sfText);

    this->setVisibility(true);
}

void Word::draw() {
    if(text.getString().isEmpty() == false) {
        if(Settings::isWordsHighlightEnabled(false) == true) {
            window.draw(this->body);
        }
        window.draw(this->text);
    }
}

void Word::update() {
    sf::Vector2f parentScale = parent->body.getScale();

    this->text.setScale(parentScale);
    this->body.setScale(parentScale);

    auto textBounds = this->text.getGlobalBounds();

    sf::Vector2f newPosition(
        parent->body.getPosition().x + this->posRatio.getX() * parent->body.getSize().x * parentScale.x - textBounds.width,
        parent->body.getPosition().y + this->posRatio.getY() * parent->body.getSize().y * parentScale.y
    );
    this->text.setPosition(newPosition);

    this->body.setPosition(newPosition);
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