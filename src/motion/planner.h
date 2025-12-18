#pragma once
#include <Arduino.h>
#include "stepper.h"
#include "corexy_kinematics.h"

class Planner {
public:
    Planner(Stepper* stepperA, Stepper* stepperB, CoreXY* kinematics);

    void moveTo(float x_mm, float y_mm, float speed_mm_per_sec);

private:
    Stepper* _stepperA;
    Stepper* _stepperB;
    CoreXY* _kinematics;

    float _curX_mm = 0;
    float _curY_mm = 0;
};
