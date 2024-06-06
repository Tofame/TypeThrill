#include <stdexcept>

#include "ComboBox.h"

#include "Button.h"
#include "../Globals.h"
#include "../ResourceManagers/FontManager.h"
#include "../Game/GameInterface.h"
#include "../Settings.h"
#include "fmt/ostream.h"

ComboBox::ComboBox(sf::Vector2f size, sf::Vector2f posRatios) {
    this->setType(COMBOBOX);

    this->body = sf::RectangleShape(size);
    this->posRatio.setValues(posRatios);

    this->setVisibility(true);
}

ComboBox::ComboBox(UIElement* parent, sf::Vector2f size, sf::Vector2f posRatios) {
    this->setType(COMBOBOX);

    this->parent = parent;

    this->body = sf::RectangleShape(size);
    this->body.setScale(1.0, 1.0);
    this->posRatio.setValues(posRatios);

    this->setVisibility(true);
}

ComboBox::~ComboBox() {
    for(auto comboButton : comboButtons) {
        this->removeElement(comboButton);
    }
}

void ComboBox::draw() {
    if(this->parent == nullptr) return;

    window.draw(this->body);
    window.draw(this->chosenText);

    if(text.getString().isEmpty() == false) {
        window.draw(this->text);
    }

    if(this->isActive() && this->isEnabled()) {
        for(auto comboButton : comboButtons) {
            comboButton->draw();
        }
    }
}

void ComboBox::update() {
    if (this->parent == nullptr) { // panelWindow is a panel with no parent, its the "father" of all panels
        throw std::runtime_error("ComboBox::update() error - ComboBox does not have a Parent");
    }

    sf::Vector2f parentScale = parent->body.getScale();

    auto textWidth = this->text.getLocalBounds().width * parentScale.x;

    sf::Vector2f newPosition(
        parent->body.getPosition().x + this->posRatio.getX() * parent->body.getSize().x * parentScale.x,
        parent->body.getPosition().y + this->posRatio.getY() * parent->body.getSize().y * parentScale.y
    );
    this->body.setPosition(newPosition);
    this->body.setScale(parentScale);

    auto moveOffsetX = 0.0f;
    if (text.getString().isEmpty() == false) {
        moveOffsetX = UIElement::offsetBodyAfterText * parentScale.x;
        this->body.move(moveOffsetX, 0);
        this->text.setPosition(newPosition);
        this->text.setScale(parentScale);
    } else {
        moveOffsetX = -(this->body.getSize().x/2 * parentScale.x);
        this->body.move(moveOffsetX, 0);
    }

    auto bodyPosition = this->body.getPosition();
    this->chosenText.setPosition(bodyPosition);
    this->chosenText.setScale(parentScale);

    for(auto comboButton : this->comboButtons) {
        comboButton->update();
        comboButton->setPosition(this->body.getPosition().x, comboButton->body.getPosition().y);
        comboButton->getText().setOrigin(-2 * parentScale.x,0);
    }
}

void ComboBox::handleClick() {
    if(isEnabled() == false)
        return;

    if(this->isActive()) {
        this->deactivate();

        for(auto comboButton : this->comboButtons) {
            if(comboButton->isMouseOver(sf::Mouse::getPosition(window))) {
                comboButton->handleClick();
                break;
            }
        }
    } else {
        this->activate();
    }
}

bool ComboBox::isMouseOver(const sf::Vector2i& mousePos) {
    auto mouseOverButton = false;

    if(isActive()) {
        for(auto comboButton : this->comboButtons) {
            if(comboButton->isMouseOver(mousePos)) {
                comboButton->setState(HOVERED);
                mouseOverButton = true;
            } else {
                comboButton->setState(DEFAULT);
            }
        }
    }

    return mouseOverButton || body.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

void ComboBox::addElementToHide() {
}

void ComboBox::getElementsToHide() {
}

sf::Text& ComboBox::getText() {
    return this->text;
}

void ComboBox::setText(sf::Text &text) {
    this->text = text;
}

void ComboBox::addComboButton(std::string buttonText, std::function<void()> const& onClick) {
    auto button = new Button();
    button->body = sf::RectangleShape({this->body.getSize().x, 30});
    button->body.setOutlineThickness(2);
    button->body.setOutlineColor(sf::Color::Black);
    if(this->comboButtons.size() % 2 == 0) {
        button->body.setFillColor(sf::Color(115, 114, 114));
    } else {
        button->body.setFillColor(sf::Color(77, 77, 77));
    }

    auto textForButton = sf::Text();
    textForButton.setFont(FontManager::Fonts["aleoItalic"]);
    textForButton.setCharacterSize(GameInterface::smallCharacterSize);
    textForButton.setFillColor(sf::Color::White);
    textForButton.setString(buttonText);
    button->setText(textForButton);

    button->setVisibility(true);
    button->onClick = onClick;

    this->addElement(button);
}

void ComboBox::addElement(Button* comboButton) {
    comboButton->setParent(this);

    // Automatic adjusting of ratios when adding to ComboBox
    float calculatedPosYRatio = 0.0f;
    if (!this->comboButtons.empty()) {
        auto lastElement = this->comboButtons.back();
        calculatedPosYRatio = lastElement->posRatio.getY() + 0.1 + (lastElement->body.getSize().y / this->body.getSize().y);
    } else {
        calculatedPosYRatio = 1 + comboButton->body.getSize().y / this->body.getSize().y;
    }

    comboButton->posRatio.setValues(this->posRatio.getX(), calculatedPosYRatio);

    comboButton->update();
    this->comboButtons.push_back(comboButton);
}

void ComboBox::removeElement(Button* comboButton) {
    auto it = std::find(this->comboButtons.begin(), comboButtons.end(), comboButton);
    if (it != comboButtons.end()) {
        delete *it;
        comboButtons.erase(it);
    }
}

Button* ComboBox::getElement(int index) {
    if (index < 0 || index >= comboButtons.size()) {
        throw std::out_of_range("Index " + std::to_string(index) + " out of range in Panel::getElement");
    }
    return this->comboButtons.at(index);
}

void ComboBox::activate() {
    active = true;
}

void ComboBox::deactivate() {
    active = false;
    this->setState(DEFAULT);
}

bool ComboBox::isActive() {
    return active;
}

sf::String ComboBox::getChosenTextString() {
    return chosenText.getString();
}

void ComboBox::setChosenText(sf::String value) {
    this->chosenText.setString(value);
}

void ComboBox::setChosenText(sf::Text value) {
    this->chosenText = value;
}