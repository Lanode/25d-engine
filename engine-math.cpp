#include <corecrt_math_defines.h>
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

float CycleAngle(float angle)
{
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0) {
        angle += 2 * M_PI;
    }
    return angle;
}

// float abs(sf::Vector2 vec)
// {
//     return std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
// }