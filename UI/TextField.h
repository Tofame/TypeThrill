#pragma once

#include <SFML/Graphics.hpp>
#include "UIElement.h"

enum FieldValueType {
    ALL = 0,
    DIGITS = 1,
    DIGITS_FLOAT = 2, // 1-9 and coma
    ALPHABET = 3
};

class TextField : public UIElement {
public:
    TextField() = default;
    TextField(sf::Vector2f& size, sf::Vector2f& position);

    void draw() override;
    void handleClick() override;
    void update() override;
    void onWriteableKeyPressed(int mode, sf::Uint32 c) override;
    bool isClicked(const sf::Vector2i& mousePos) override;

    void updatePointLinePosition(int offsetX);

    sf::Text& getText();
    void setText(sf::Text& text);

    sf::String getInputString();
    void setInput(sf::String value);
    void setInput(sf::Text value);

    void move(float x, float y) override;
    void setPosition(float x, float y) override;

    FieldValueType getAllowedValues();
    void setAllowedValues(FieldValueType type);

    sf::Text pointLine;
private:
    sf::Text text;
    sf::Text input;
    FieldValueType allowedValues;
};