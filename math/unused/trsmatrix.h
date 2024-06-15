#ifndef TRSMATRIX_H
#define TRSMATRIX_H

#include "matrix.h"
#include "vector.h"

template <typename T>
class TRSMatrix : public Matrix<T>
{
public:
    TRSMatrix();

    void Combine(const TRSMatrix<T>& transform);
    void Translate(const Vector3<T>& translation);
    void Scale(const Vector3<T>& scaling);
    void Rotate(const Vector3<T>& rotation);
};

#endif // TRSMATRIX_H
