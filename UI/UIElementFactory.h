#pragma once
#include "Button.h"
#include "Checkbox.h"
#include "Panel.h"
#include "TextField.h"
#include "ComboBox.h"
#include "UIElement.h"
#include "DynamicTextLabel.h"
#include "../Game/GameInterface.h"
#include "../Settings.h"
#include "../Globals.h"
#include "../ResourceManagers/FontManager.h"

class UIElementFactory {
public:
    static sf::Color ColorBlue;
    static sf::Color ColorGreen;
    static sf::Color ColorGray;
    static sf::Color ColorGrayDarkLO;
    static sf::Color ColorGrayDark;

    static ComboBox* createComboBox(const std::string& fieldValue, sf::Vector2f posRatios, const std::string& textValue);

    static DynamicTextLabel* createStatisticsDynamicLabel(sf::Vector2f ratios, std::function<std::string()> const& onUpdateString);

    static DynamicTextLabel* createInfoDynamicLabel(sf::Vector2f ratios, std::function<std::string()> const& onUpdateString);

    static Checkbox* createMenuCheckbox(float sizeMultiplier, sf::Vector2f posRatios,
        std::string textValue, bool defaultEnabled, std::function<void(bool value)> const& onCheckboxUpdate);

    static TextField* createTextField(const std::string& fieldValue, sf::Vector2f posRatios,
        std::string textValue, std::wstring pattern);

    static TextField* createTextField(const std::string& fieldValue, sf::Vector2f posRatios,
        std::string textValue, std::wstring pattern, std::function<void()> const& onTextFieldUpdate);

    static Button* createMenuButton(const std::string& name, const std::function<void()>& onClick, sf::Vector2f posRatios);

    static Button* createMenuButton(const std::string& name, const std::function<void()>& onClick, sf::Vector2f posRatios, sf::Vector2f size);

    static Panel* createPanel(UIElement* parent, sf::Vector2f size, sf::Vector2f posRatios, PanelType panelType);
};
