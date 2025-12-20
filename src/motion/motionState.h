// IT: extension hpp
#pragma once

// IT could this be a pure data class?
// e.g. https://nikitablack.github.io/post/functional_programming_in_c++_by_example/
class MotionState {
public:
    MotionState();

    float getX() const;
    float getY() const;
    void setPosition(float x, float y);

    // Future: add velocity, acceleration, etc.

private:
    float _x_mm; // IT: naming convention - and what does the extension _mm mean?
    float _y_mm;
};
