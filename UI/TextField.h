#pragma once

#include <SFML/Graphics.hpp>
#include "UIElement.h"
#include <regex>

class TextField : public UIElement {
public:
    TextField() = default;
    TextField(sf::Vector2f& size, sf::Vector2f& posRatios);

    void draw() override;
    void handleClick() override;
    void update() override;
    void onWriteableKeyPressed(int mode, sf::Uint32 c) override;
    bool isMouseOver(const sf::Vector2i& mousePos) override;

    // Source of knowledge: https://stackoverflow.com/questions/20353210/what-is-the-purpose-of-stdfunction-and-how-do-i-use-it
    // void <= doesn't return anything, () <= empty brackets, takes no parameters. We can now save such lambdas into TextField now.
    std::function<void()> onTextFieldUpdate = []() -> void {};

    void updatePointLinePosition(int offsetX);

    sf::Text& getText();
    void setText(sf::Text& text);

    sf::String getInputString();
    void setInput(sf::String value);
    void setInput(sf::Text value);

    void move(float x, float y) override;
    void setPosition(float x, float y) override;

    std::wregex getPattern();
    void setPattern(std::wstring pattern);

    float getOffsetBodyAfterText();
    void setOffsetBodyAfterText(float value);

    sf::Text pointLine;
private:
    sf::Text text;
    sf::Text input;
    std::wregex pattern;

    // -1 means its ignored in getOffsetBodyAfterText() and static value from UIElement is taken
    float offsetBodyAfterText = -1;
};