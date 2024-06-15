#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

template <typename T>
class Matrix {
protected:
    std::vector<std::vector<T>> data;
    size_t rows;
    size_t cols;

public:
    Matrix(size_t rows, size_t cols, T value = 0.0);
    Matrix(const Matrix& other);
    Matrix(std::initializer_list<std::initializer_list<T>> lst);
    size_t numRows() const;
    size_t numCols() const;
    T& operator()(size_t row, size_t col);
    const T& operator()(size_t row, size_t col) const;
    Matrix operator+(const Matrix& other) const;
    Matrix operator-() const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(T scalar) const;
    Matrix operator*(const Matrix& other) const;
};

#endif // MATRIX_H
