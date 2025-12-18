#pragma once

class MotionState {
public:
    MotionState();

    float getX() const;
    float getY() const;
    void setPosition(float x, float y);

    // Future: add velocity, acceleration, etc.

private:
    float _x_mm;
    float _y_mm;
};
