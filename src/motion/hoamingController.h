#pragma once
#include <Arduino.h>
#include "stepper.h"
#include "corexy_kinematics.h"
#include "step_converter.h"
#include "MotionState.h"
#include <TMCStepper.h>

enum Direction {
    UP,    // +Y
    DOWN,  // -Y
    LEFT,  // -X
    RIGHT  // +X
};

class HomingController {
public:
    HomingController(Stepper* stepperA, Stepper* stepperB, TMC2209Stepper* driverA, TMC2209Stepper* driverB, StepConverter* converterA, StepConverter* converterB, CoreXY* kinematics, MotionState* state);

    void findLimit(Direction direction, float speed_mm_per_sec, float stallGuard_threshold);

private:
    Stepper* _stepperA;
    Stepper* _stepperB;
    TMC2209Stepper* _driverA;
    TMC2209Stepper* _driverB;
    StepConverter* _converterA;
    StepConverter* _converterB;
    CoreXY* _kinematics;
    MotionState* _state;
};