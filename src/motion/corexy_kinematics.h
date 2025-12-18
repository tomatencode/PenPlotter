#pragma once
#include <Arduino.h>

struct CoreXYCoords {
    long A;
    long B;
};
struct CartesianCoords {
    float x_mm;
    float y_mm;
};

class CoreXY {
public:
    CoreXY(float stepsPerMM);

    CoreXYCoords toCoreXY(float x_mm, float y_mm) const;
    CartesianCoords toCartesian(long A_steps, long B_steps) const;

private:
    float _stepsPerMM;
};
