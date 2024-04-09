#ifndef ENGINEMATH_H
#define ENGINEMATH_H

#include <compare>

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

#endif // ENGINEMATH_H
