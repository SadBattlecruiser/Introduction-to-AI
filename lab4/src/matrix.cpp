#ifndef MATRIX
#define MATRIX

#include <vector>
#include <iostream>

using namespace std;

template <typename T>
class matrix {
public:
  matrix(size_t N, size_t M);
  size_t get_n() const;
  size_t get_m() const;
  void fill(const T value);
  void print() const;
  vector<T>& operator[](size_t idx_i);
  const T& ij(size_t idx_i, size_t idx_j) const;
  matrix<T>& operator*(const matrix<T>& r_matr) const;
  matrix<T>& operator+(const matrix<T>& r_matr) const;
  matrix<T>& operator-(const matrix<T>& r_matr) const;
  matrix<T>& slice(size_t i_b, size_t i_e, size_t j_b, size_t j_e);
  matrix<T>& reshape(size_t N_new, size_t M_new);
private:
  size_t N_;
  size_t M_;
  vector<vector<T> > arr_;
};

template <typename T>
matrix<T>::matrix(size_t N, size_t M) {
  N_ = N;
  M_ = M;
  //arr_ = new vector<vector<T> >[N];
  arr_ = vector<vector<T> >(N);
  for (size_t i = 0; i < N_; i++) {
    arr_[i].resize(M);
  }
}

template <typename T>
size_t matrix<T>::get_n() const {
  return N_;
}

template <typename T>
size_t matrix<T>::get_m() const {
  return M_;
}

template <typename T>
void matrix<T>::print() const {
  cout << "shape: " << N_ << " " << M_ << endl;
  for (size_t i = 0; i < N_; i++) {
    for (size_t j = 0; j < M_; j++) {
      cout << arr_[i][j] << ' ';
    }
    cout << endl;
  }
}

template <typename T>
void matrix<T>::fill(const T value) {
  for (size_t i = 0; i < N_; i++) {
    for (size_t j = 0; j < M_; j++) {
      arr_[i][j] = value;
    }
  }
}

template <typename T>
vector<T>& matrix<T>::operator[](size_t idx_N) {
  return arr_[idx_N];
}

template <typename T>
const T& matrix<T>::ij(size_t idx_i, size_t idx_j) const {
  return arr_[idx_i][idx_j];
}

template <typename T>
matrix<T>& matrix<T>::operator*(const matrix<T>& r_matr) const {
  size_t r_N = N_;
  size_t r_M = r_matr.get_m();
  matrix<T>& ret_matr = *(new matrix<T>(r_N, r_M));
  for (size_t i = 0; i < r_N; i++) {
    for (size_t j = 0; j < r_M; j++) {
      for (size_t k = 0; k < M_; k++) {
        ret_matr[i][j] += arr_[i][k] * r_matr.ij(k, j);
      }
    }
  }
  return ret_matr;
}

template <typename T>
matrix<T>& matrix<T>::operator+(const matrix<T>& r_matr) const {
  matrix<T>& ret_matr = *(new matrix<T>(N_, M_));
  for (size_t i = 0; i < N_; i++) {
    for (size_t j = 0; j < M_; j++) {
      ret_matr[i][j] = arr_[i][j] + r_matr.ij(i, j);
    }
  }
  return ret_matr;
}

template <typename T>
matrix<T>& matrix<T>::operator-(const matrix<T>& r_matr) const {
  matrix<T>& ret_matr = *(new matrix<T>(N_, M_));
  for (size_t i = 0; i < N_; i++) {
    for (size_t j = 0; j < M_; j++) {
      ret_matr[i][j] = arr_[i][j] - r_matr.ij(i, j);
    }
  }
  return ret_matr;
}

template <typename T>
matrix<T>& matrix<T>::slice(size_t i_b, size_t i_e, size_t j_b, size_t j_e) {
  size_t r_N = i_e - i_b;
  size_t r_M = j_e - j_b;
  matrix<T>& ret_matr = *(new matrix<T>(r_N, r_M));
  for (size_t i = 0; i < r_N; i++) {
    for (size_t j = 0; j < r_M; j++) {
      ret_matr[i][j] = arr_[i_b + i][j_b + j];
    }
  }
  return ret_matr;
}

template <typename T>
matrix<T>& matrix<T>::reshape(size_t N_new, size_t M_new) {
  if (N_ * M_ != N_new * M_new) {
    throw "BAD SIZES IN RESHAPE\n";
  }
  matrix<T>& ret_matr = *(new matrix<T>(N_new, M_new));
  for (size_t i = 0; i < N_new; i++) {
    for (size_t j = 0; j < M_new; j++) {
      size_t idx = i * M_new + j;  // Одномерная координата
      size_t i_old = idx / M_;
      size_t j_old = idx % M_;
      ret_matr[i][j] = arr_[i_old][j_old];
      // std::cout << "i_" << i << " j_ " << j << std::endl;
      // std::cout << "io" << i_old << " jo " << j_old << std::endl;
    }
  }
  return ret_matr;
}

#endif
