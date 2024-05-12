#pragma once

#include "Button.h"

class ButtonFactory {
public:
    std::map<std::string, std::vector<Button>> static Buttons;

    void static setupButtons();
    Button static createButton();
    Button static createMenuButton(const std::string& text, const std::function<void()>& onClick);
};