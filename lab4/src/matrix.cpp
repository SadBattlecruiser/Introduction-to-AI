#ifndef MATRIX
#define MATRIX

#include <vector>
#include <iostream>

using namespace std;

template <typename T>
class matrix {
public:
  matrix(size_t N, size_t M);
  matrix(const matrix<T>& orig);
  ~matrix();
  size_t get_n() const;
  size_t get_m() const;
  void fill(const T value);
  void print() const;
  T* operator[](size_t idx_i);
  const T& ij(size_t idx_i, size_t idx_j) const;
  matrix<T>& operator*(const matrix<T>& r_matr) const;
  matrix<T>& operator*(const T& val) const;
  matrix<T>& operator+(const matrix<T>& r_matr) const;
  matrix<T>& operator-(const matrix<T>& r_matr) const;
  matrix<T>& operator+=(const matrix<T>& r_matr);
  matrix<T>& slice(size_t i_b, size_t i_e, size_t j_b, size_t j_e) const;
  matrix<T>& reshape(size_t N_new, size_t M_new) const;
  matrix<T>& transpose() const;
private:
  size_t N_;
  size_t M_;
  T* arr_;
};

template <typename T>
matrix<T>::matrix(size_t N, size_t M) {
  // std::cout << "matrix constructor begin" << std::endl;
  N_ = N;
  M_ = M;
  arr_ = new T[N_*M_];
  // std::cout << "matrix constructor vv create" << std::endl;
  // std::cout << "matrix constructor begin" << std::endl;
}

template <typename T>
matrix<T>::matrix(const matrix<T>& orig) {
  N_ = orig.N_;
  M_ = orig.M_;
  arr_ = new T[N_*M_];
  for (size_t i = 0; i < N_ * M_; i++) {
    arr_[i] = orig.arr_[i];
  }
}

template <typename T>
matrix<T>::~matrix() {
  delete[] arr_;
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
      cout << arr_[i * M_ + j] << ' ';
    }
    cout << endl;
  }
}

template <typename T>
void matrix<T>::fill(const T value) {
  for (size_t i = 0; i < N_; i++) {
    for (size_t j = 0; j < M_; j++) {
      //std::cout << i * M_ + j << std::endl;
      arr_[i * M_ + j] = value;
    }
  }
}

template <typename T>
T* matrix<T>::operator[](size_t idx_N) {
  return arr_ + idx_N * M_;
}

template <typename T>
const T& matrix<T>::ij(size_t idx_i, size_t idx_j) const {
  return arr_[idx_i * M_ + idx_j];
}

template <typename T>
matrix<T>& matrix<T>::operator*(const matrix<T>& r_matr) const {
  if (M_ != r_matr.get_n()) {
    std::cout << "BAD SIZES IN operator*\n";
    throw "BAD SIZES IN operator*\n";
  }
  size_t r_N = N_;
  size_t r_M = r_matr.get_m();
  matrix<T>& ret_matr = *(new matrix<T>(r_N, r_M));
  ret_matr.fill(0.);
  for (size_t i = 0; i < r_N; i++) {
    for (size_t j = 0; j < r_M; j++) {
      for (size_t k = 0; k < M_; k++) {
        ret_matr[i][j] += arr_[i * M_ + k] * r_matr.ij(k, j);
      }
    }
  }
  return ret_matr;
}

template <typename T>
matrix<T>& matrix<T>::operator*(const T& val) const {
  matrix<T>& ret_matr = *(new matrix<T>(N_, M_));
  ret_matr.fill(0.);
  for (size_t i = 0; i < N_; i++) {
    for (size_t j = 0; j < M_; j++) {
      ret_matr[i][j] = arr_[i * M_ + j] * val;
    }
  }
  return ret_matr;
}

template <typename T>
matrix<T>& matrix<T>::operator+(const matrix<T>& r_matr) const {
  if (N_ != r_matr.get_n() || M_ != r_matr.get_m()) {
    std::cout << "BAD SIZES IN operator+\n";
    throw "BAD SIZES IN operator+\n";
  }
  matrix<T>& ret_matr = *(new matrix<T>(N_, M_));
  ret_matr.fill(0.);
  for (size_t i = 0; i < N_; i++) {
    for (size_t j = 0; j < M_; j++) {
      ret_matr[i][j] = arr_[i * M_ + j] + r_matr.ij(i, j);
    }
  }
  return ret_matr;
}

template <typename T>
matrix<T>& matrix<T>::operator-(const matrix<T>& r_matr) const {
  if (N_ != r_matr.get_n() || M_ != r_matr.get_m()) {
    std::cout << "BAD SIZES IN operator-\n";
    throw "BAD SIZES IN operator-\n";
  }
  matrix<T>& ret_matr = *(new matrix<T>(N_, M_));
  ret_matr.fill(0.);
  for (size_t i = 0; i < N_; i++) {
    for (size_t j = 0; j < M_; j++) {
      ret_matr[i][j] = arr_[i * M_ + j] - r_matr.ij(i, j);
    }
  }
  return ret_matr;
}

template <typename T>
matrix<T>& matrix<T>::operator+=(const matrix<T>& r_matr) {
  if (N_ != r_matr.get_n() || M_ != r_matr.get_m()) {
    std::cout << "BAD SIZES IN operator+=\n";
    throw "BAD SIZES IN operator+=\n";
  }
  for (size_t i = 0; i < N_; i++) {
    for (size_t j = 0; j < M_; j++) {
      arr_[i * M_ + j] += r_matr.ij(i, j);
    }
  }
  return *this;
}

template <typename T>
matrix<T>& matrix<T>::slice(size_t n_b, size_t n_e, size_t m_b, size_t m_e) const {
  size_t r_N = n_e - n_b;
  size_t r_M = m_e - m_b;
  matrix<T>& ret_matr = *(new matrix<T>(r_N, r_M));
  ret_matr.fill(0.);
  for (size_t i = 0; i < r_N; i++) {
    for (size_t j = 0; j < r_M; j++) {
      ret_matr[i][j] = arr_[(n_b + i)*M_ + (m_b + j)];
    }
  }
  return ret_matr;
}

template <typename T>
matrix<T>& matrix<T>::reshape(size_t N_new, size_t M_new) const{
  if (N_ * M_ != N_new * M_new) {
    std::cout << "BAD SIZES IN reshape\n";
    throw "BAD SIZES IN reshape\n";
  }
  matrix<T>& ret_matr = *(new matrix<T>(N_new, M_new));
  ret_matr.fill(0.);
  for (size_t i = 0; i < N_new; i++) {
    for (size_t j = 0; j < M_new; j++) {
      //std::cout << i << ' ' << j << ' ' << arr_[i * M_new + j] << std::endl;
      ret_matr[i][j] = arr_[i * M_new + j];
    }
  }
  return ret_matr;
}

template <typename T>
matrix<T>& matrix<T>::transpose() const{
  matrix<T>& ret_matr = *(new matrix<T>(M_, N_));
  ret_matr.fill(0.);
  for (size_t i = 0; i < N_; i++) {
    for (size_t j = 0; j < M_; j++) {
      ret_matr[j][i] = arr_[i * M_ + j];
    }
  }
  return ret_matr;
}

#endif
