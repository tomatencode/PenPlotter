#include "planner.h"
#include <math.h>

Planner::Planner(Stepper* stepperA, Stepper* stepperB, StepConverter* converterA, StepConverter* converterB, CoreXY* kinematics)
    : _stepperA(stepperA), _stepperB(stepperB), _converterA(converterA), _converterB(converterB), _kinematics(kinematics) {}

void Planner::moveTo(float x_mm, float y_mm, float speed_mm_per_sec) {
    float deltaX = x_mm - _curX_mm;
    float deltaY = y_mm - _curY_mm;

    if (deltaX == 0 && deltaY == 0) return; // no movement needed

    // Convert positions to steps using converters
    long currentX_steps = _converterA->mmToSteps(_curX_mm);
    long currentY_steps = _converterB->mmToSteps(_curY_mm);
    long targetX_steps = _converterA->mmToSteps(x_mm);
    long targetY_steps = _converterB->mmToSteps(y_mm);

    // Get current and target motor steps using kinematics
    CoreXYCoords currentSteps = _kinematics->toCoreXY(currentX_steps, currentY_steps);
    CoreXYCoords targetSteps = _kinematics->toCoreXY(targetX_steps, targetY_steps);

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
    Serial.print("dirA: "); Serial.print(dirA); Serial.print(" dirB: "); Serial.println(dirB);

    // Step counters
    long stepCountA = 0;
    long stepCountB = 0;
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

    // Update current position
    _curX_mm = x_mm;
    _curY_mm = y_mm;
}