#include "stepper.h"

Stepper::Stepper(uint8_t stepPin, uint8_t dirPin, TMC2209Stepper* driver)
    : _stepPin(stepPin), _dirPin(dirPin), _driver(driver) {}

void Stepper::enable() { digitalWrite(_stepPin, LOW); }
void Stepper::disable() { digitalWrite(_stepPin, HIGH); }
void Stepper::setCurrent(float mA) { _driver->rms_current(mA); }
void Stepper::setMicrosteps(int ms) { _driver->microsteps(ms); }
void Stepper::setDirection(bool dir) { digitalWrite(_dirPin, dir ? HIGH : LOW); }

void Stepper::step() {
    digitalWrite(_stepPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_stepPin, LOW);
}
