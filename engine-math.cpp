#include <cmath>
#include "engine-math.h"

using namespace eng;

Vector Vector::operator+(Vector a)
{
    return {x+a.x, y+a.y};
}

Vector Vector::operator*(float a)
{
    return {x*a, y*a};
}

bool Vector::operator==(Vector a)
{
    return x == a.x && y == a.y;
}

Vector Vector::Rotate(float degrees)
{
    return {
        (float)(cos(degrees)*x - sin(degrees)*y),
        (float)(sin(degrees)*x + cos(degrees)*y)
    };
}
