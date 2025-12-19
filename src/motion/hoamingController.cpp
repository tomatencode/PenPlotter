#include "hoamingController.h"

HomingController::HomingController(Stepper* stepperA, Stepper* stepperB, TMC2209Stepper* driverA, TMC2209Stepper* driverB, StepConverter* converterA, StepConverter* converterB, CoreXY* kinematics, MotionState* state)
    : _stepperA(stepperA), _stepperB(stepperB), _driverA(driverA), _driverB(driverB), _converterA(converterA), _converterB(converterB), _kinematics(kinematics), _state(state) {}

void HomingController::findLimit(Direction direction, float speed_mm_per_min, float stallGuard_threshold) {
    // Determine direction vectors
    float deltaX = 0.0f;
    float deltaY = 0.0f;
    switch (direction) {
        case UP:
            deltaY = 1.0f;
            break;
        case DOWN:
            deltaY = -1.0f;
            break;
        case LEFT:
            deltaX = -1.0f;
            break;
        case RIGHT:
            deltaX = 1.0f;
            break;
    }

    // Use kinematics to determine motor directions
    long currentX_steps = 0;
    long currentY_steps = 0;
    long targetX_steps = _converterA->mmToSteps(deltaX);
    long targetY_steps = _converterB->mmToSteps(deltaY);

    CoreXYCoords currentSteps = _kinematics->toCoreXY(currentX_steps, currentY_steps);
    CoreXYCoords targetSteps = _kinematics->toCoreXY(targetX_steps, targetY_steps);

    bool dirA = (targetSteps.A - currentSteps.A) >= 0;
    bool dirB = (targetSteps.B - currentSteps.B) >= 0;

    // Set directions
    _stepperA->setDirection(dirA);
    _stepperB->setDirection(dirB);

    // set stallGuard threshold
    _driverA->SGTHRS((uint8_t)stallGuard_threshold);
    _driverB->SGTHRS((uint8_t)stallGuard_threshold);

    // Calculate step interval for continuous stepping
    long stepsPerMM = _converterA->mmToSteps(1.0f);  // Approximate
    float time_per_mm = 60.0f / speed_mm_per_min;  // speed_mm_per_min is in mm/min
    unsigned long stepInterval_us = (unsigned long)(time_per_mm * 1000000.0f / stepsPerMM);

    unsigned long lastStepTime = micros();
    long stepCount = 0;

    while (true) {  // Step continuously until stall
        unsigned long currentTime = micros();
        if (currentTime - lastStepTime >= stepInterval_us) {
            // Step both motors
            _stepperA->step();
            _stepperB->step();
            stepCount++;

            // Update Position
            float stepA_mm = (dirA ? 1.0f : -1.0f) * _converterA->stepsToMm(1);
            float stepB_mm = (dirB ? 1.0f : -1.0f) * _converterB->stepsToMm(1);
            CartesianCoords deltaCoords = _kinematics->toCartesian(stepA_mm, stepB_mm);
            float newX = _state->getX() + deltaCoords.x;
            float newY = _state->getY() + deltaCoords.y;
            _state->setPosition(newX, newY);

            // Check stall via UART every 10 steps
            if (stepCount % 10 == 0) {
                long sgA = _driverA->SG_RESULT();
                long sgB = _driverB->SG_RESULT();
                Serial.print("SG A: "); Serial.print(sgA); Serial.print(" B: "); Serial.println(sgB);
                if (false /*sgA < stallGuard_threshold || sgB < stallGuard_threshold*/) {
                    Serial.println("Stall detected, limit found");
                    // Position is already updated to the stall point
                    break;
                }
            }
            
            lastStepTime = currentTime;
        }
    }
    _driverA->SGTHRS(0);
    _driverB->SGTHRS(0);
}