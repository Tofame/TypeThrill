#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "Button.h"
#include "UIElement.h"

class ButtonFactory {
public:
    std::map<std::string, std::vector<Button>> static Buttons;

    void static setupButtons();
    Button static createMenuButton(const std::string& name, const std::function<void()>& onClick);

    std::unique_ptr<UIElement> static createButton();
};