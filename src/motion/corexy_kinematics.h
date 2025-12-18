#pragma once
#include <Arduino.h>

struct CoreXYCoords {
    float A;
    float B;
};
struct CartesianCoords {
    float x;
    float y;
};

class CoreXY {
public:
    CoreXY();

    CoreXYCoords toCoreXY(float x, float y) const;
    CartesianCoords toCartesian(float A, float B) const;
};
