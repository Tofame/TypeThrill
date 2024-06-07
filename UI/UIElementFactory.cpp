#include "UIElementFactory.h"

#include "../ResourceManagers/GameSaveManager.h"

sf::Color UIElementFactory::ColorBlue = sf::Color(52, 194, 199);
sf::Color UIElementFactory::ColorGreen = sf::Color(52, 235, 88);
sf::Color UIElementFactory::ColorGray = sf::Color(105,104,102);
sf::Color UIElementFactory::ColorGrayDarkLO = sf::Color(64,64,64, 120);
sf::Color UIElementFactory::ColorGrayDark = sf::Color(54, 54, 54);

UIElementFactory* UIElementFactory::UIElementFactory_= nullptr;

UIElementFactory * UIElementFactory::getInstance() {
    if(UIElementFactory_ == nullptr){
        UIElementFactory_ = new UIElementFactory();
    }
    return UIElementFactory_;
}

// Returns a ComboBox UIElement and right after that sets a properties that we want for it like a font, size etc..
ComboBox* UIElementFactory::createComboBox(const std::string& fieldValue, sf::Vector2f posRatios, const std::string& textValue) {
    sf::Vector2f size = {200, 30};
    auto element = new ComboBox(size, posRatios);

    auto& rect = element->body;
    rect.setFillColor(UIElementFactory::ColorGrayDark);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(3);

    if(textValue.empty() == false) {
        auto text = sf::Text();
        text.setFont(FontManager::Fonts["times"]);
        text.setCharacterSize(GameInterface::smallCharacterSize);

        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);

        text.setString(textValue);

        element->setText(text);
    }

    if(fieldValue.empty() == false) {
        auto chosenText = sf::Text();
        chosenText.setFont(FontManager::Fonts["aleoItalic"]);
        chosenText.setCharacterSize(GameInterface::smallCharacterSize);
        chosenText.setFillColor(sf::Color::White);
        chosenText.setString(fieldValue);

        element->setChosenText(chosenText);
    }

    return element;
}

// Returns a DynamicTextLabel "Statistics" meaning it's size and aligntype properties are unique for this method.
// (e.g. different character size than other dynamic text labels).
DynamicTextLabel* UIElementFactory::createStatisticsDynamicLabel(sf::Vector2f ratios, std::function<std::string()> const& onUpdateString) {
    auto element = new DynamicTextLabel(ratios, onUpdateString);

    element->getText().setCharacterSize(GameInterface::mediumLiteCharacterSize);
    element->setAlignType(ALIGN_NONE);

    return element;
}

// Returns a DynamicTextLabel "Info" meaning it's size and aligntype properties are unique for this method
// (e.g. different character size than other dynamic text labels).
DynamicTextLabel* UIElementFactory::createInfoDynamicLabel(sf::Vector2f ratios, std::function<std::string()> const& onUpdateString) {
    auto element = new DynamicTextLabel(ratios, onUpdateString);

    element->getText().setCharacterSize(GameInterface::smallCharacterSize);
    element->setAlignType(ALIGN_NONE);

    return element;
}

// Returns a Checkbox in Menu's style
// Which for this project is a blue filled rectangle with white outline
Checkbox* UIElementFactory::createMenuCheckbox(float sizeMultiplier, sf::Vector2f posRatios, std::string textValue, bool defaultEnabled, std::function<void(bool value)> const& onCheckboxUpdate) {
    auto element = new Checkbox(sizeMultiplier, posRatios, defaultEnabled);

    element->onCheckboxUpdate = onCheckboxUpdate;

    if(textValue.empty() == false) {
        auto text = sf::Text();
        text.setFont(FontManager::Fonts["times"]);
        text.setCharacterSize(GameInterface::smallCharacterSize);

        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);

        text.setString(textValue);

        element->setText(text);
    }

    return element;
}

// Returns a TextField that doesn't expect initalizing the lambda for it. (Meaning textfield by default wont do anything special)
TextField* UIElementFactory::createTextField(const std::string& fieldValue, sf::Vector2f posRatios, std::string textValue, std::wstring pattern) {
    sf::Vector2f size = {200, 30};
    auto element = new TextField(size, posRatios);

    // Set allowed input - a regex pattern (e.g. only DIGITS (1-9) allowed)
    element->setPattern(pattern);

    auto& rect = element->body;
    rect.setFillColor(UIElementFactory::ColorGray);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(3);

    if(textValue.empty() == false) {
        auto text = sf::Text();
        text.setFont(FontManager::Fonts["times"]);
        text.setCharacterSize(GameInterface::smallCharacterSize);

        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);

        text.setString(textValue);

        element->setText(text);
    }

    auto input = sf::Text();
    input.setFont(FontManager::Fonts["aleoItalic"]);
    input.setCharacterSize(GameInterface::smallCharacterSize);
    input.setFillColor(sf::Color::White);
    input.setString(fieldValue);

    // I-line, Cursour etc. <- Its the line that blinks and indicates where your next letter will be
    element->pointLine.setFont(FontManager::Fonts["times"]);
    element->pointLine.setCharacterSize(GameInterface::smallCharacterSize);
    element->pointLine.setFillColor(sf::Color::White);
    element->pointLine.setString("|");

    element->setInput(input);

    return element;
}

// Returns a TextField that expects a lambda for it. (Meaning textfield will react to onTextFieldUpdate (text input) in the way we want it to)
TextField* UIElementFactory::createTextField(const std::string& fieldValue, sf::Vector2f posRatios, std::string textValue, std::wstring pattern, std::function<void()> const& onTextFieldUpdate) {
    auto element = UIElementFactory::createTextField(fieldValue, posRatios, textValue, pattern);
    element->onTextFieldUpdate = onTextFieldUpdate;

    return element;
}

// Returns a Button in Menu's style
// Which for this project is a blue filled rectangle with white outline
// Doesn't accept size parameter and this method does it inside.
Button* UIElementFactory::createMenuButton(const std::string& name, const std::function<void()>& onClick, sf::Vector2f posRatios) {
    sf::Vector2f size = {200, 50};
    return createMenuButton(name, onClick, posRatios, size);
}

// Returns a Button in Menu's style
// Which for this project is a blue filled rectangle with white outline
// Allows for size parameter
Button* UIElementFactory::createMenuButton(const std::string& name, const std::function<void()>& onClick, sf::Vector2f posRatios, sf::Vector2f size) {
    auto element = new Button(size, posRatios, onClick);

    auto& rect = element->body;
    rect.setFillColor(UIElementFactory::ColorBlue);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(4);

    auto text = sf::Text();
    text.setFont(FontManager::Fonts["jaro"]);
    text.setCharacterSize(GameInterface::smallCharacterSize);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1);
    text.setString(name);
    text.setPosition(rect.getPosition());
    text.move(rect.getSize().x/2 - text.getGlobalBounds().width/2, rect.getSize().y/2 - text.getGlobalBounds().height + text.getGlobalBounds().height/3);
    element->setText(text);

    return element;
}

// Returns a Panel in the Project's Style
// Which is transparent body with green outline
Panel* UIElementFactory::createPanel(UIElement* parent, sf::Vector2f size, sf::Vector2f posRatios, PanelType panelType) {
    auto panel = new Panel(parent, size, posRatios);

    panel->setPanelType(panelType);

    auto& rect = panel->body;
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Green);
    rect.setOutlineThickness(3);

    return panel;
}

Button* UIElementFactory::createSaveSlot(const std::string& name, sf::Vector2f posRatios, sf::Vector2f size, int slotIndex) {
    auto element = new Button(size, posRatios);
    element->onClick = [slotIndex]() -> void { GameSaveManager::getInstance()->saveGame(slotIndex); };

    auto& rect = element->body;
    rect.setFillColor(sf::Color(48, 48, 47));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(3);

    auto text = sf::Text();
    text.setFont(FontManager::Fonts["jaro"]);
    text.setCharacterSize(GameInterface::smallCharacterSize);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1);
    text.setString(name);
    text.setPosition(rect.getPosition());
    text.move(rect.getSize().x/2 - text.getGlobalBounds().width/2, rect.getSize().y/2 - text.getGlobalBounds().height + text.getGlobalBounds().height/3);
    element->setText(text);

    element->setVisibility(false);
    return element;
}

DynamicTextLabel* UIElementFactory::createSaveSlotDynamicLabel(sf::Vector2f ratios, int slotIndex, Panel* panelToFitInto, Button* saveButton) {
    auto onUpdateString = [slotIndex]() -> std::string {
        return GameSaveManager::getInstance()->getSlotDescription(slotIndex);
    };

    auto element = new DynamicTextLabel(ratios, onUpdateString);

    element->getText().setCharacterSize(GameInterface::smallCharacterSize);
    element->setAlignType(ALIGN_NONE);

    // This adds description label to the panel so it will be drawn
    panelToFitInto->addElement(element);

    // And then we set parent of the said label to the element (Save button)
    // so the update() function can position it based on element's position.
    element->setParent(saveButton);

    element->setVisibility(false);
    return element;
}