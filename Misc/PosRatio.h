#pragma once

struct PosRatio {
    PosRatio() {
        this->x = 0;
        this->y = 0;
    }

    PosRatio(float x, float y) {
        this->x = x;
        this->y = y;
    }

    float getX() {
        return this->x;
    }
    float getY() {
        return this->y;
    }

    void setX(float x) {
        this->x = x;
    }
    void setY(float y) {
        this->y = y;
    }

    // Sets X Position Ratio and Y Position Ratio values
    void setValues(float x, float y) {
        this->x = x;
        this->y = y;
    };
    // Sets X Position Ratio and Y Position Ratio values
    void setValues(sf::Vector2f vec) {
        this->x = vec.x;
        this->y = vec.y;
    }

private:
    float x;
    float y;
};