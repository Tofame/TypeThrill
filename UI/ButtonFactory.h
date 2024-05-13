#pragma once

#include <map>
#include <string>
#include <vector>

#include "Button.h"

class ButtonFactory {
public:
    std::map<std::string, std::vector<Button>> static Buttons;

    void static setupButtons();
    Button static createButton();
    Button static createMenuButton(const std::string& name, const std::function<void()>& onClick);
};