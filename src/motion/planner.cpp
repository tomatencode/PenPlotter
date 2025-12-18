#include "planner.h"
#include <math.h>

Planner::Planner(Stepper* stepperA, Stepper* stepperB, CoreXY* kinematics)
    : _stepperA(stepperA), _stepperB(stepperB), _kinematics(kinematics) {}

void Planner::moveTo(float x_mm, float y_mm, float speed_mm_per_sec) {
    float deltaX = x_mm - _curX_mm;
    float deltaY = y_mm - _curY_mm;

    if (deltaX == 0 && deltaY == 0) return; // no movement needed

    // Get current and target motor steps
    CoreXYCoords currentSteps = _kinematics->toCoreXY(_curX_mm, _curY_mm);
    CoreXYCoords targetSteps = _kinematics->toCoreXY(x_mm, y_mm);

    long deltaA = targetSteps.A - currentSteps.A;
    long deltaB = targetSteps.B - currentSteps.B;

    // Determine directions and absolute steps
    bool dirA = deltaA >= 0;
    bool dirB = deltaB >= 0;
    long stepsA = abs(deltaA);
    long stepsB = abs(deltaB);

    long totalSteps = max(stepsA, stepsB);
    if (totalSteps == 0) return;

    // Calculate timing
    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
    float time_sec = distance / speed_mm_per_sec;
    unsigned long stepInterval_us = (unsigned long)((time_sec * 1000000.0f) / totalSteps);

    // set directions
    _stepperA->setDirection(dirA);
    _stepperB->setDirection(dirB);

    // Step counters
    long stepCountA = 0;
    long stepCountB = 0;
    Serial.print("stepsA: "); Serial.print(stepsA); Serial.print(" stepsB: "); Serial.println(stepsB);
    unsigned long lastStepTime = micros();
    long step = 0;

    // Perform the move
    while (step < totalSteps) {
        unsigned long currentTime = micros();
        if (currentTime - lastStepTime >= stepInterval_us) {
            // Step A
            if (stepCountA * totalSteps < stepsA * step) {
                _stepperA->step();
                stepCountA++;
            }
            // Step B
            if (stepCountB * totalSteps < stepsB * step) {
                _stepperB->step();
                stepCountB++;
            }
            lastStepTime = currentTime;
            step++;
        }
    }

    Serial.print("Moved to ("); Serial.print(x_mm); Serial.print(", "); Serial.print(y_mm); Serial.println(")");
    Serial.print("Final Steps A: "); Serial.print(stepCountA); Serial.print(" B: "); Serial.println(stepCountB);

    // Update current position
    _curX_mm = x_mm;
    _curY_mm = y_mm;
}