// C++ header names should have extensions hpp
#pragma once
#include <TMCStepper.h>

class StepConverter {
public:
    StepConverter(float stepsPerMM_full, TMC2209Stepper* driver);

    long mmToSteps(float mm) const;
    float stepsToMm(long steps) const;
private:
    float _stepsPerMM_full;
    TMC2209Stepper* _driver;
};