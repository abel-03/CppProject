#pragma once

#include <iostream>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 public:
  Matrix() = default;
  Matrix(std::vector<std::vector<T>>& matrix2);
  Matrix(T elem);
  Matrix(Matrix<N, M, T>& matrix2);

  Matrix<N, M, T> operator+(const Matrix<N, M, T>& matrix2) const;
  Matrix<N, M, T> operator-(const Matrix<N, M, T>& matrix2) const;
  Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& matrix2);
  Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& matrix2);
  Matrix<N, M, T>& operator=(const Matrix<N, M, T>& matrix2);

  Matrix<N, M, T> operator*(T elem) const;
  template <size_t R>
  Matrix<N, R, T> operator*(const Matrix<M, R, T>& matrix2) const;

  Matrix<M, N, T> Transposed();
  T Trace();

  T operator()(size_t row, size_t column) const;
  T& operator()(size_t row, size_t column);

  template <size_t P, size_t Y>
  bool operator==(const Matrix<P, Y, T>& matrix2);
  private:
    std::vector<std::vector<T>> matrix_ =
      std::vector<std::vector<T>>(N, std::vector<T>(M, T()));
};

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator=(const Matrix<N, M, T>& matrix2) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      this->matrix_[i][j] = matrix2.matrix_[i][j];
    }
  }
  return *this;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(std::vector<std::vector<T>>& matrix2) {
  this->matrix_ = matrix2;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(T elem) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      this->matrix_[i][j] = elem;
    }
  }
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>::Matrix(Matrix<N, M, T>& matrix2) {
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      this->matrix_[i][j] = matrix2.matrix_[i][j];
    }
  }
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator+(
    const Matrix<N, M, T>& matrix2) const {
  Matrix<N, M, T> res;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      res.matrix_[i][j] = this->matrix_[i][j] + matrix2.matrix_[i][j];
    }
  }
  return res;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator+=(const Matrix<N, M, T>& matrix2) {
  *this = *this + matrix2;
  return *this;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator-(
    const Matrix<N, M, T>& matrix2) const {
  Matrix<N, M, T> res;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      res.matrix_[i][j] = this->matrix_[i][j] - matrix2.matrix_[i][j];
    }
  }
  return res;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T>& Matrix<N, M, T>::operator-=(const Matrix<N, M, T>& matrix2) {
  *this = *this - matrix2;
  return *this;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> Matrix<N, M, T>::operator*(T elem) const {
  Matrix<N, M, T> res;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      res.matrix_[i][j] = this->matrix_[i][j] * elem;
    }
  }
  return res;
}

template <size_t N, size_t M, typename T>
template <size_t R>
Matrix<N, R, T> Matrix<N, M, T>::operator*(
    const Matrix<M, R, T>& matrix2) const {
  Matrix<N, R, T> res;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < R; ++j) {
      for (size_t k = 0; k < M; ++k) {
        res(i, j) += (*this)(i, k) * matrix2(k, j);
      }
    }
  }
  return res;
}

template <size_t N, size_t M, typename T>
Matrix<M, N, T> Matrix<N, M, T>::Transposed() {
  Matrix<M, N, T> res;
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      res(i, j) = (*this)(j, i);
    }
  }
  return res;
}

template <size_t N, size_t M, typename T>
T Matrix<N, M, T>::Trace() {
  T res = 0;
  if (N == M) {
    for (size_t i = 0; i < N; ++i) {
      res += this->matrix_[i][i];
    }
    return res;
  }
}

template <size_t N, size_t M, typename T>
T Matrix<N, M, T>::operator()(size_t row, size_t column) const {
  return this->matrix_[row][column];
}

template <size_t N, size_t M, typename T>
T& Matrix<N, M, T>::operator()(size_t row, size_t column) {
  return this->matrix_[row][column];
}


template <size_t N, size_t M, typename T>
template <size_t P, size_t Y>
bool Matrix<N, M, T>::operator==(const Matrix<P, Y, T>& matrix2) {
    if (N == P && M == Y) {
      for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
          if (this->matrix_[i][j] != matrix2.matrix_[i][j]) {
            return false;
          }
        }
      }
      return true;
    }
    return false;
}