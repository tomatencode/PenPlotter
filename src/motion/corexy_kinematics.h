#pragma once
#include <Arduino.h>

struct CoreXYCoords {
    long A;
    long B;
};
struct CartesianCoords {
    float x;
    float y;
};

class CoreXY {
public:
    CoreXY();

    CoreXYCoords toCoreXY(float x, float y) const;
    CartesianCoords toCartesian(long A, long B) const;
};
