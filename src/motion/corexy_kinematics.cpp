#include "corexy_kinematics.h"
#include <math.h>

CoreXY::CoreXY() {}

CoreXYCoords CoreXY::toCoreXY(float x, float y) const {

    CoreXYCoords c;
    c.A = lround(x + y);
    c.B = lround(x - y);
    return c;
}
CartesianCoords CoreXY::toCartesian(long A, long B) const {

    CartesianCoords c;
    c.x = (A + B) * 0.5f;
    c.y = (A - B) * 0.5f;
    return c;
}