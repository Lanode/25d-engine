#include "vector.h"

template <typename T>
Vector<T>::Vector(size_t dimension)
    : Matrix<T>(dimension, 1)
{}

template<typename T>
Vector2<T>::Vector2()
    : Vector<T>(2)
{}

template<typename T>
Vector3<T>::Vector3()
    : Vector<T>(3)
{}

template class Vector2<int>;
template class Vector2<float>;
template class Vector3<int>;
template class Vector3<float>;
