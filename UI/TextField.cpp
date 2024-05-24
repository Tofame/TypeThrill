#include "TextField.h"
#include "../Globals.h"
#include "../Game/GameInterface.h"
#include "fmt/Core.h"

// Help methods that check if unicode corresponds to digit etc.
bool checkUnicode_DIGIT(sf::Uint32 unicode) {
    return (std::iswdigit(static_cast<wchar_t>(unicode)));
}

bool checkUnicode_ALPHABET(sf::Uint32 unicode) {
    return std::iswalpha(static_cast<wchar_t>(unicode));
}
// End of help methods

TextField::TextField(sf::Vector2f& size, sf::Vector2f& position) {
    this->body = sf::RectangleShape(size);
    this->body.setPosition(position);
    this->visibility = true;
    this->setPosRatios(0.1, 0.1);
}

void TextField::draw() {
    window.draw(this->body);
    window.draw(this->input);

    if(text.getString().isEmpty() == false) {
        window.draw(this->text);
    }

    if(getState() == FOCUSED) {
        auto currentColor = this->pointLine.getFillColor();
        auto alpha = currentColor.a;
        if(alpha <= 0) {
            currentColor.a = 255;
            this->pointLine.setFillColor(currentColor);
        } else {
            currentColor.a -= 10;
            this->pointLine.setFillColor(currentColor);
        }

        window.draw(this->pointLine);
    }
}

void TextField::handleClick() {
    if(this->getState() == FOCUSED) {
        this->setState(DEFAULT);
        return;
    }

    // Disable all focused elements
    for(auto panel : GameInterface::panels) {
        if(panel->isVisible()) {
            for(auto uielement : panel->UIElements) {
                if(uielement->getState() == FOCUSED)
                    uielement->setState(DEFAULT);
            }
        }
    }

    this->setState(FOCUSED);
}

void TextField::update() {
    sf::Vector2f parentScale = parent->body.getScale();

    auto textWidth = this->text.getLocalBounds().width * parentScale.x;

    sf::Vector2f newPosition(
        parent->body.getPosition().x + this->posXRatio * parent->body.getSize().x * parentScale.x,
        parent->body.getPosition().y + this->posYRatio * parent->body.getSize().y * parentScale.y
    );
    this->body.setPosition(newPosition);
    this->body.setScale(parentScale);

    if (text.getString().isEmpty() == false) {
        this->body.move(textWidth + 6, 0);
        this->text.setPosition(newPosition);
        this->text.setScale(parentScale);
    }

    auto bodyPosition = this->body.getPosition();
    this->input.setPosition(bodyPosition);
    this->input.setScale(parentScale);

    this->updatePointLinePosition(0);
}

void TextField::updatePointLinePosition(int offsetX) {
    this->pointLine.setPosition(this->input.getPosition());
    this->pointLine.move(this->input.getGlobalBounds().width + offsetX, -4);

    this->pointLine.setScale(this->parent->body.getScale());
}

void TextField::onWriteableKeyPressed(int mode, sf::Uint32 unicode) {
    auto inputString = getInputString();

    if(mode == 0) {
        if(inputString.isEmpty()) {
            return;
        }

        inputString.erase(inputString.getSize() - 1);
    } else {
        bool canAddToString = false;
        auto type = this->getAllowedValues();

        auto size = inputString.getSize();

        switch(type) {
            case ALL: canAddToString = true; break;
            case DIGITS: canAddToString = checkUnicode_DIGIT(unicode); break;
            case DIGITS_FLOAT_R:
            {
                auto max = 3; // Maximum chars number in this type (DIGITS_FLOAT_RESTRICTED)
                if(size > max)
                    break;

                if(size == 1 && unicode != U'.') // we are adding 2nd one which means it must be a coma
                    break;

                if((size == 0 || size >= 2 && size < max) && !checkUnicode_DIGIT(unicode)) // 0.23 <- 0first digit, 1coma etc.
                    break;

                canAddToString = true;
                break;
            }
            case ALPHABET: canAddToString = checkUnicode_ALPHABET(unicode); break;
            default:
                throw std::runtime_error("Unknown type used in onWriteableKeyPressed: " + type);
        }

        if(canAddToString == true) {
            inputString += unicode;
        }
    }

    this->input.setString(inputString);
    this->updatePointLinePosition(0);
}

bool TextField::isClicked(const sf::Vector2i& mousePos) {
    return body.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

sf::Text& TextField::getText() {
    return this->text;
}

void TextField::setText(sf::Text &text) {
    this->text = text;
}

sf::String TextField::getInputString() {
    return input.getString();
}

void TextField::setInput(sf::String value) {
    this->input.setString(value);
}

void TextField::setInput(sf::Text value) {
    this->input = value;
}

void TextField::move(float x, float y) {
    this->body.move(x, y);
    this->text.move(x, y);
    this->pointLine.move(x, y);
}

void TextField::setPosition(float x, float y) {
    this->body.setPosition(x, y);
    this->text.setPosition(x, y);
    this->pointLine.setPosition(x, y);
}

FieldValueType TextField::getAllowedValues() {
    return this->allowedValues;
}

void TextField::setAllowedValues(FieldValueType type) {
    this->allowedValues = type;
}