#include <numbers>
#include "functions.h"

float CycleAngle(float angle)
{
    angle = fmod(angle, 2 * std::numbers::pi);
    if (angle < 0) {
        angle += 2 * std::numbers::pi;
    }
    return angle;
}

float clip(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

template <typename T>
sf::Vector3<T> ToVector3(const sf::Vector2<T>& vector2)
{
    return sf::Vector3<T>(vector2.x, vector2.y, T());
}

template sf::Vector3<float> ToVector3(const sf::Vector2<float>& vector2);

template <typename T>
sf::Vector2<T> ToVector2(const sf::Vector3<T>& vector3)
{
    return sf::Vector2<T>(vector3.x, vector3.y);
}

template sf::Vector2<float> ToVector2(const sf::Vector3<float>& vector3);
