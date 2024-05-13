#pragma once

#include <map>
#include <string>
#include <vector>

#include "Checkbox.h"

class CheckboxFactory {
public:
    std::map<std::string, std::vector<Checkbox>> static Buttons;

    void static setupCheckboxes();
    Checkbox static createCheckbox();
};