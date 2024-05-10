#pragma once

#include "Button.h"
#include "UIButton.h"
#include <memory>

class ButtonFactory {
public:
    std::map<std::string, std::unique_ptr<Button>> static Buttons;

    void static setupButtons();
    std::unique_ptr<Button> static createUIButton();
};
