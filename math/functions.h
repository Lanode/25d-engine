#ifndef ENGINEMATH_H
#define ENGINEMATH_H

#include <SFML/Graphics.hpp>

float CycleAngle(float angle);
float clip(float n, float lower, float upper);

template <typename T>
sf::Vector3<T> ToVector3(const sf::Vector2<T>& vector2);

template <typename T>
sf::Vector2<T> ToVector2(const sf::Vector3<T>& vector3);

#endif // ENGINEMATH_H
