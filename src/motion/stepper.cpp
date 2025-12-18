#include "stepper.h"

Stepper::Stepper(uint8_t stepPin, uint8_t dirPin)
    : _stepPin(stepPin), _dirPin(dirPin) {}

void Stepper::enable() { digitalWrite(_stepPin, LOW); }
void Stepper::disable() { digitalWrite(_stepPin, HIGH); }
void Stepper::setDirection(bool dir) { digitalWrite(_dirPin, dir ? HIGH : LOW); }

void Stepper::step() {
    digitalWrite(_stepPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_stepPin, LOW);
}
