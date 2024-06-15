#include <stdexcept>
#include "matrix.h"

template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols, T value)
    : rows(rows), cols(cols), data(rows, std::vector<double>(cols, value)) {}

template <typename T>
Matrix<T>::Matrix(const Matrix& other)
    : rows(other.rows), cols(other.cols), data(other.data) {}

template<typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T> > lst)
{
    rows = lst.size();
    cols = lst[0].size();
    for (auto &x : lst)
        if (x.size() < cols)
            throw std::invalid_argument("Row dimensions do not match");

    data.resize(rows);
    auto it=lst.begin();
    for (int i=0; i<rows; i++, it++) {
        std::vector<T> row(cols);
        std::copy(it->begin(), it->end(), row);
        data.push_back(row);
    }
}

template <typename T>
size_t Matrix<T>::numRows() const {
    return rows;
}

template <typename T>
size_t Matrix<T>::numCols() const {
    return cols;
}

template <typename T>
T& Matrix<T>::operator()(size_t row, size_t col) {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return data[row][col];
}

template <typename T>
const T& Matrix<T>::operator()(size_t row, size_t col) const {
    if (row >= rows || col >= cols) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return data[row][col];
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix dimensions do not match");
    }
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result(i, j) = (*this)(i, j) + other(i, j);
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator-() const {
    return (*this) * (-1);
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& other) const {
    return (*this) + (-other);
}

template <typename T>
Matrix<T> Matrix<T>::operator*(T scalar) const {
    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result(i, j) = (*this)(i, j) * scalar;
        }
    }
    return result;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& other) const {
    if (cols != other.rows) {
        throw std::invalid_argument("Matrix dimensions do not match for multiplication");
    }
    Matrix result(rows, other.cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < other.cols; ++j) {
            for (size_t k = 0; k < cols; ++k) {
                result(i, j) += (*this)(i, k) * other(k, j);
            }
        }
    }
    return result;
}
