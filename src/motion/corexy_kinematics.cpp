#include "corexy_kinematics.h"
#include <math.h>

CoreXY::CoreXY(float stepsPerMM_XY)
    : _stepsPerMM(stepsPerMM_XY) {}

CoreXYCoords CoreXY::toCoreXY(float x_mm, float y_mm) const {
    float x_steps = x_mm * _stepsPerMM;
    float y_steps = y_mm * _stepsPerMM;

    CoreXYCoords c;
    c.A = lround(x_steps + y_steps);
    c.B = lround(x_steps - y_steps);
    return c;
}
CartesianCoords CoreXY::toCartesian(long A_steps, long B_steps) const {
    float A_mm = A_steps / _stepsPerMM;
    float B_mm = B_steps / _stepsPerMM;

    CartesianCoords c;
    c.x_mm = (A_mm + B_mm) * 0.5f;
    c.y_mm = (A_mm - B_mm) * 0.5f;
    return c;
}