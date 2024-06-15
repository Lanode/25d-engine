#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <array>
#include <SFML/Graphics.hpp>

typedef std::array<std::array<float, 4>, 4> MatrixArray;

class Transform
{
private:
    MatrixArray matrix;
public:
    Transform();
    Transform(MatrixArray matrix);

    void Combine(const Transform& transform);
    void Translate(const sf::Vector3f& t);
    void Scale(const sf::Vector3f& s);
    void Rotate(const sf::Vector3f& r);
    sf::Vector3f TransformPoint(const sf::Vector3f& p);
};

#endif // TRANSFORM_H
