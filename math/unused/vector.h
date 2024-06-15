#ifndef VECTOR_H
#define VECTOR_H

#include "matrix.h"

template <typename T>
class Vector : public Matrix<T>
{
public:
    Vector(size_t dimension);
};

template <typename T>
class Vector2 : public Vector<T>
{
public:
    Vector2();
};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;

template <typename T>
class Vector3 : public Vector<T>
{
public:
    Vector3();
};

typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;


#endif // VECTOR_H
