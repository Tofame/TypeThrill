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

// https://refactoring.guru/design-patterns/singleton/cpp/example
// We go with Naive Singleton for now
class UIElementFactory {
protected:
    UIElementFactory() = default;
    static UIElementFactory* UIElementFactory_;

public:
    // Singleton is not cloneable
    UIElementFactory(UIElementFactory& other) = delete;
    // Singleton can't be assigned
    void operator=(const UIElementFactory&) = delete;

    static UIElementFactory* getInstance();

    static sf::Color ColorBlue;
    static sf::Color ColorGreen;
    static sf::Color ColorGray;
    static sf::Color ColorGrayDarkLO;
    static sf::Color ColorGrayDark;

    ComboBox* createComboBox(const std::string& fieldValue, sf::Vector2f posRatios, const std::string& textValue);

    DynamicTextLabel* createStatisticsDynamicLabel(sf::Vector2f ratios, std::function<std::string()> const& onUpdateString);

    DynamicTextLabel* createInfoDynamicLabel(sf::Vector2f ratios, std::function<std::string()> const& onUpdateString);

    Checkbox* createMenuCheckbox(float sizeMultiplier, sf::Vector2f posRatios,
        std::string textValue, bool defaultEnabled, std::function<void(bool value)> const& onCheckboxUpdate);

    TextField* createTextField(const std::string& fieldValue, sf::Vector2f posRatios,
        std::string textValue, std::wstring pattern);

    TextField* createTextField(const std::string& fieldValue, sf::Vector2f posRatios,
        std::string textValue, std::wstring pattern, std::function<void()> const& onTextFieldUpdate);

    Button* createMenuButton(const std::string& name, const std::function<void()>& onClick, sf::Vector2f posRatios);

    Button* createMenuButton(const std::string& name, const std::function<void()>& onClick, sf::Vector2f posRatios, sf::Vector2f size);

    Panel* createPanel(UIElement* parent, sf::Vector2f size, sf::Vector2f posRatios, PanelType panelType);

    Button* createSaveSlot(const std::string& name, sf::Vector2f posRatios, sf::Vector2f size, int slotIndex);
    DynamicTextLabel* createSaveSlotDynamicLabel(sf::Vector2f ratios, int slotIndex, Panel* panelToFitInto, Button* saveButton);
};