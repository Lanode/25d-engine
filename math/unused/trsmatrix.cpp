#include "trsmatrix.h"

template <typename T>
TRSMatrix<T>::TRSMatrix()
    : Matrix<T>(4, 4)
{}

template<typename T>
void TRSMatrix<T>::Combine(const TRSMatrix<T> &trs)
{
    *this = (*this) * trs;
}

template<typename T>
void TRSMatrix<T>::Translate(const Vector3<T>& translation)
{
    Matrix<T> translationMatrix {
        {1 ,2 ,3, translation(0, 0)}
    };
}

template<typename T>
void TRSMatrix<T>::Scale(const Vector3<T>& scaling)
{

}

template<typename T>
void TRSMatrix<T>::Rotate(const Vector3<T>& rotation)
{

}
