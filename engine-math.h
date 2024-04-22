#ifndef ENGINEMATH_H
#define ENGINEMATH_H

#if _MSC_VER
#include <corecrt_math_defines.h>
#endif
#include "cfloat"
#include <cmath>
#include <compare>

#define RAD_TO_DEG 180/M_PI
#define DEG_TO_RAD M_PI/180

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

namespace eng {
    struct Vector
    {
        float x;
        float y;

        auto operator <=> (const Vector&) const = default;
        bool operator == (Vector a);
        Vector operator+(Vector a);
        Vector operator*(float a);
        Vector Rotate(float degrees);
    };
}

float CycleAngle(float angle);

#endif // ENGINEMATH_H
