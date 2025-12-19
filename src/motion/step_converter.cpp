#include "step_converter.h"
#include <cmath>

StepConverter::StepConverter(float stepsPerMM_full, TMC2209Stepper* driver)
    : _stepsPerMM_full(stepsPerMM_full), _driver(driver) {}

long StepConverter::mmToSteps(float mm) const {
    int ms = _driver->microsteps();
    return lround(mm * ms / _stepsPerMM_full);
}

float StepConverter::stepsToMm(long steps) const {
    int ms = _driver->microsteps();
    return steps * _stepsPerMM_full / ms;
}