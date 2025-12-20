#pragma once
#include <Arduino.h> // IT: this file does not really depend on Arduino.h, right? So please remove this dependency


struct CoreXYCoords {
    // IT: Nice! This is a pure data class.     
    float A; // IT: naming convention
    float B;
};
struct CartesianCoords {
    float x;
    float y;
};

class CoreXY {
public:
    CoreXY(); // IT Modern C++ allows  CoreXY() = default;

    CoreXYCoords toCoreXY(float x, float y) const;
    CartesianCoords toCartesian(float A, float B) const;
};
