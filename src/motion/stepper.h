#pragma once
#include <Arduino.h>
#include <TMCStepper.h>

class Stepper {
public:
    Stepper(uint8_t stepPin, uint8_t dirPin);

    void enable();
    void disable();
    void setDirection(bool dir);
    void step();

private:
    uint8_t _stepPin;
    uint8_t _dirPin;
    TMC2209Stepper* _driver;
};
