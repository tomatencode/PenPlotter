#pragma once
#include <Arduino.h> // IT: ordering of includes
#include "stepper.h"
#include "corexy_kinematics.h"
#include "step_converter.h"
#include "MotionState.h"
#include <TMCStepper.h>

enum Direction { // IT: naming convention - e.g. use that one: https://google.github.io/styleguide/cppguide.html
    UP,    // +Y
    DOWN,  // -Y
    LEFT,  // -X
    RIGHT  // +X
};

class HomingController {
public:
    HomingController(Stepper* stepperA, Stepper* stepperB, TMC2209Stepper* driverA, TMC2209Stepper* driverB, CoreXY* kinematics);

    void moveToLimit(Direction direction, float speed_full_stps_per_s, float stallGuard_threshold);

private:
    Stepper* _stepperA; // naming convention, avoid hard pointers
    Stepper* _stepperB;
    TMC2209Stepper* _driverA;
    TMC2209Stepper* _driverB;
    CoreXY* _kinematics;
};