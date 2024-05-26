#pragma once
#include <functional>

#include "UIElement.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"

class Checkbox : public UIElement {
public:
    Checkbox() = default;
    Checkbox(float sizeMultiplier, sf::Vector2f posRatios, bool defaultEnabled);

    bool isEnabled() const;
    void enable();
    void disable();

    void draw() override;
    void handleClick() override;
    void update() override;

    // Source of knowledge: https://stackoverflow.com/questions/20353210/what-is-the-purpose-of-stdfunction-and-how-do-i-use-it
    // void <= doesn't return anything, () <= empty brackets, takes no parameters. We can now save such lambdas into TextField now.
    std::function<void(bool value)> onCheckboxUpdate = [](bool value) -> void {};

    void setState(UIElementState state) override;

    bool isMouseOver(const sf::Vector2i& mousePos) override;

    float getSizeMultiplier();

    sf::Text& getText();
    void setText(sf::Text& text);

private:
    sf::Text text;
    sf::Sprite checkBoxSprite;
    sf::Texture texture;
    float sizeMultiplier;
    bool enabled = false;
};
