#include "hoamingController.h"

HomingController::HomingController(Stepper* stepperA, Stepper* stepperB, TMC2209Stepper* driverA, TMC2209Stepper* driverB, CoreXY* kinematics)
    : _stepperA(stepperA), _stepperB(stepperB), _driverA(driverA), _driverB(driverB), _kinematics(kinematics) {}

void HomingController::moveToLimit(Direction direction, float speed_full_stps_per_s, float stallGuard_threshold) {
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

    CoreXYCoords currentSteps = _kinematics->toCoreXY(0, 0);
    CoreXYCoords targetSteps = _kinematics->toCoreXY(deltaX, deltaY);

    bool dirA = (targetSteps.A - currentSteps.A) >= 0;
    bool dirB = (targetSteps.B - currentSteps.B) >= 0;

    // set stallGuard threshold
    _driverA->SGTHRS((uint8_t)stallGuard_threshold);
    _driverB->SGTHRS((uint8_t)stallGuard_threshold);

    // set directions
    _driverA->VACTUAL(speed_full_stps_per_s * _driverA->microsteps() * (dirA ? 1 : -1));
    _driverB->VACTUAL(speed_full_stps_per_s * _driverB->microsteps() * (dirB ? 1 : -1));

    unsigned long lastCheckTime = micros();
    long stepCount = 0;

    delay(500); // Give some time to start moving
    while (true) {  // Step continuously until stall
        unsigned long currentTime = micros();
        if (currentTime - lastCheckTime >= 10000) {
           
            long sgA = _driverA->SG_RESULT();
            long sgB = _driverB->SG_RESULT();
            Serial.print("SG A: "); Serial.print(sgA); Serial.print(" B: "); Serial.println(sgB);
            if (sgA < stallGuard_threshold || sgB < stallGuard_threshold) {
                Serial.println("Stall detected, limit found");
                // Position is already updated to the stall point
                break;
            }

            lastCheckTime = currentTime;
        }
    }

    // Stop motors
    _driverA->VACTUAL(0);
    _driverB->VACTUAL(0);

    // Reset stallGuard thresholds
    _driverA->SGTHRS(0);
    _driverB->SGTHRS(0);
}