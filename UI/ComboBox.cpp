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
    for(auto uielement : UIElements) {
        this->removeElement(uielement);
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
        for(auto uielement : UIElements) {
            uielement->draw();
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

    for(auto uielement : this->UIElements) {
        uielement->update();
        uielement->setPosition(this->body.getPosition().x, uielement->body.getPosition().y);
        uielement->getText().setOrigin(-2 * parentScale.x,0);
    }
}

void ComboBox::handleClick() {
    if(isEnabled() == false)
        return;

    if(this->isActive()) {
        this->deactivate();

        for(auto uielement : this->UIElements) {
            if(uielement->isMouseOver(sf::Mouse::getPosition(window))) {
                uielement->handleClick();
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
        for(auto uielement : this->UIElements) {
            if(uielement->isMouseOver(mousePos)) {
                uielement->setState(HOVERED);
                mouseOverButton = true;
            } else {
                uielement->setState(DEFAULT);
            }
        }
    }

    return mouseOverButton || body.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
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
    if(this->UIElements.size() % 2 == 0) {
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

void ComboBox::addElement(Button* UIElement) {
    UIElement->setParent(this);

    // Automatic adjusting of ratios when adding to ComboBox
    float calculatedPosYRatio = 0.0f;
    if (!this->UIElements.empty()) {
        auto lastElement = this->UIElements.back();
        calculatedPosYRatio = lastElement->posRatio.getY() + 0.1 + (lastElement->body.getSize().y / this->body.getSize().y);
    } else {
        calculatedPosYRatio = 1 + UIElement->body.getSize().y / this->body.getSize().y;
    }

    UIElement->posRatio.setValues(this->posRatio.getX(), calculatedPosYRatio);

    UIElement->update();
    this->UIElements.push_back(UIElement);
}

void ComboBox::removeElement(Button* UIElement) {
    auto it = std::find(this->UIElements.begin(), UIElements.end(), UIElement);
    if (it != UIElements.end()) {
        delete *it;
        UIElements.erase(it);
    }
}

Button* ComboBox::getElement(int index) {
    if (index < 0 || index >= UIElements.size()) {
        throw std::out_of_range("Index " + std::to_string(index) + " out of range in Panel::getElement");
    }
    return this->UIElements.at(index);
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