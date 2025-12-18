#include <Arduino.h>
#include <TMCStepper.h>
#include "motion/stepper.h"
#include "motion/planner.h"
#include "motion/step_converter.h"
#include "motion/MotionState.h"
#include "motion/hoamingController.h"

// === Pins and driver ===
#define STEP_PIN_A 19
#define DIR_PIN_A 4
#define STEP_PIN_B 18
#define DIR_PIN_B 5
#define EN_PIN 23

#define DRIVER_TX_PIN 17
#define DRIVER_RX_PIN 16
#define R_SENSE 0.11f

HardwareSerial driverSerial(1);

TMC2209Stepper driverA(&driverSerial, R_SENSE, 0);
TMC2209Stepper driverB(&driverSerial, R_SENSE, 2);
Stepper stepA(STEP_PIN_A, DIR_PIN_A);
Stepper stepB(STEP_PIN_B, DIR_PIN_B);
StepConverter converterA(320.0f, &driverA);
StepConverter converterB(320.0f, &driverB);
CoreXY kinematics;
MotionState state;
Planner planner(&stepA, &stepB, &converterA, &converterB, &kinematics, &state);
HomingController homing(&stepA, &stepB, &driverA, &driverB, &converterA, &converterB, &kinematics, &state);

float stallGuard_threshold = 30.0f;
float speed_mm_per_s = 0.3;

void setup() {
    Serial.begin(115200);
    pinMode(STEP_PIN_A, OUTPUT);
    pinMode(DIR_PIN_A, OUTPUT);
    pinMode(STEP_PIN_B, OUTPUT);
    pinMode(DIR_PIN_B, OUTPUT);
    pinMode(EN_PIN, OUTPUT);

    digitalWrite(EN_PIN, LOW);

    driverSerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);
    // === INIT DRIVER A ===
    driverA.begin();
    driverA.toff(5);
    driverA.rms_current(1000);
    driverA.microsteps(16);
    driverA.en_spreadCycle(false);
    driverA.pwm_autoscale(true);

    // === INIT DRIVER B ===
    driverB.begin();
    driverB.toff(5);
    driverB.rms_current(1000);
    driverB.microsteps(16);
    driverB.en_spreadCycle(false);
    driverB.pwm_autoscale(true);
}

void loop() {
    // Homing sequence to find workspace dimensions
    Serial.println("Homing to RIGHT...");
    homing.findLimit(RIGHT, speed_mm_per_s, stallGuard_threshold);
    float maxX = state.getX();
    Serial.print("Max X: "); Serial.println(maxX);

    Serial.println("Homing to LEFT...");
    homing.findLimit(LEFT, speed_mm_per_s, stallGuard_threshold);
    float minX = state.getX();
    Serial.print("Min X: "); Serial.println(minX);

    Serial.println("Homing to UP...");
    homing.findLimit(UP, speed_mm_per_s, stallGuard_threshold);
    float maxY = state.getY();
    Serial.print("Max Y: "); Serial.println(maxY);

    Serial.println("Homing to DOWN...");
    homing.findLimit(DOWN, speed_mm_per_s, stallGuard_threshold);
    float minY = state.getY();
    Serial.print("Min Y: "); Serial.println(minY);

    float width = maxX - minX;
    float height = maxY - minY;
    Serial.print("Workspace Width: "); Serial.print(width); Serial.print(" Height: "); Serial.println(height);

    // Calculate center rectangle (80% size, same aspect ratio)
    float rectWidth = width * 0.8;
    float rectHeight = height * 0.8;
    float centerX = (maxX + minX) / 2.0;
    float centerY = (maxY + minY) / 2.0;

    float blX = centerX - rectWidth / 2.0;
    float blY = centerY - rectHeight / 2.0;
    float brX = centerX + rectWidth / 2.0;
    float brY = centerY - rectHeight / 2.0;
    float trX = centerX + rectWidth / 2.0;
    float trY = centerY + rectHeight / 2.0;
    float tlX = centerX - rectWidth / 2.0;
    float tlY = centerY + rectHeight / 2.0;

    Serial.println("Starting rectangle tracing...");

    while (true) {
        planner.moveTo(blX, blY, speed_mm_per_s);
        planner.moveTo(brX, brY, speed_mm_per_s);
        planner.moveTo(trX, trY, speed_mm_per_s);
        planner.moveTo(tlX, tlY, speed_mm_per_s);
    }
}
