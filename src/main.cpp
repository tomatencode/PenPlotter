#include <Arduino.h>
#include <TMCStepper.h>
#include "motion/stepper.h"
#include "motion/planner.h"
#include "motion/step_converter.h"

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
Planner planner(&stepA, &stepB, &converterA, &converterB, &kinematics);

float squareSize_mm = 2.0;
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
    driverA.rms_current(800);
    driverA.microsteps(16);
    driverA.en_spreadCycle(false);
    driverA.pwm_autoscale(true);

    // === INIT DRIVER B ===
    driverB.begin();
    driverB.toff(5);
    driverB.rms_current(800);
    driverB.microsteps(16);
    driverB.en_spreadCycle(false);
    driverB.pwm_autoscale(true);
}

void loop() {
    planner.moveTo(squareSize_mm/2, squareSize_mm/2, speed_mm_per_s);
    while (true) {
        planner.moveTo(-squareSize_mm/2, squareSize_mm/2, speed_mm_per_s);
        planner.moveTo(-squareSize_mm/2, -squareSize_mm/2, speed_mm_per_s);
        planner.moveTo(squareSize_mm/2, -squareSize_mm/2, speed_mm_per_s);
        planner.moveTo(squareSize_mm/2, squareSize_mm/2, speed_mm_per_s);
    }
}
