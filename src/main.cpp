#include <Arduino.h>
#include <TMCStepper.h>
#include "motion/stepper.h"
#include "motion/corexy_kinematics.h"
#include "motion/planner.h"

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
TMC2209Stepper driverB(&driverSerial, R_SENSE, 1);

Stepper stepA(STEP_PIN_A, DIR_PIN_A, &driverA);
Stepper stepB(STEP_PIN_B, DIR_PIN_B, &driverB);
Planner planner(&stepA, &stepB, new CoreXY(1.0f));

float squareSize = 20000.0;
float stepsPerSecond = 12000.0;

void setup() {
    Serial.begin(115200);
    pinMode(STEP_PIN_A, OUTPUT);
    pinMode(DIR_PIN_A, OUTPUT);
    pinMode(STEP_PIN_B, OUTPUT);
    pinMode(DIR_PIN_B, OUTPUT);
    pinMode(EN_PIN, OUTPUT);

    digitalWrite(EN_PIN, LOW);

    driverSerial.begin(115200, SERIAL_8N1, DRIVER_RX_PIN, DRIVER_TX_PIN);
    driverA.begin(); driverB.begin();
    stepA.enable();
    stepB.enable();
    stepA.setCurrent(800.0f);
    stepB.setCurrent(800.0f);
    stepA.setMicrosteps(16);
    stepB.setMicrosteps(16);
}

void loop() {
    planner.moveTo(squareSize/2, 0, stepsPerSecond);
    while (true) {
        planner.moveTo(-squareSize/2, 0, stepsPerSecond);
        planner.moveTo(squareSize/2, 0, stepsPerSecond);
    }
}
