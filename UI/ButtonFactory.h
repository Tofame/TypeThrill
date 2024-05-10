#pragma once

#include "Button.h"
#include "UIButton.h"

class ButtonFactory {
public:
    std::map<std::string, Button*> static Buttons;

    void static setupButtons();
    void static createUIButton(std::string name);
};
