#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

#include "Button.h"

class UIButton final : public Button {
public:
    UIButton();
    UIButton(const sf::Vector2f& size, const sf::Vector2f& position, const std::function<void()>& onClick);

    void handleClick() const override;
};