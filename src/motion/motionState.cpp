#include "MotionState.h"

MotionState::MotionState() : _x_mm(0.0f), _y_mm(0.0f) {}

float MotionState::getX() const {
    return _x_mm;
}

float MotionState::getY() const {
    return _y_mm;
}

void MotionState::setPosition(float x, float y) {
    _x_mm = x;
    _y_mm = y;
}