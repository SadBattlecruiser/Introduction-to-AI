#include "network.hpp"

double mse(matrix<double> v1, matrix<double> v2) {
  double sum = 0;
  size_t n =v1.get_n();
  for (size_t i = 0; i < n; i++) {
    sum += (v1[i][0] - v2[i][0]);
  }
  return sum / n;
}

double sum(matrix<double> v) {
  double sum = 0;
  size_t n =v.get_n();
  for (size_t i = 0; i < n; i++) {
    sum += v[i][0];
  }
  return sum;
}

matrix<double> pow2(matrix<double> v) {
  size_t n = v.get_n();
  matrix<double> ret(n, 1);
  for (size_t i = 0; i < n; i++) {
    ret[i][0] = v[i][0] * v[i][0];
  }
  return ret;
}

network::network(size_t size0, size_t size1, size_t size2)
  : W1_(size1, size0), W2_(size2, size1) {
  W1_.fill(0.2);
  W2_.fill(0.2);
  size0_ = size0;
  size1_ = size1;
  size2_ = size2;
}

matrix<double>& network::out(const matrix<double>& in) const {
  return W2_ * (W1_ * in);
}

void network::fit(const std::vector< matrix<double>> orig) {
  size_t n = orig[0].get_n();
  size_t total_n = n * orig.size();

  // double total_error = 0;
  // double curr_error = 0;
  // double mse = 0;
  std::vector<matrix<double> > outs;
  std::vector<matrix<double> > errors;
  for (size_t i = 0; i < n; i++) {
    // Получаем выходное изображение
    auto out_curr = this->out(orig[i]);
    outs.push_back(out_curr);
    delete &out_curr;
    // Ошибки на нем
    auto errors_curr =orig[i] - outs[i];
    errors.push_back(errors_curr);
    delete &errors_curr;
  }
  //mse = total_error / total_n;

  // Чистим память?
  // for (size_t i = 0; i < n; i++) {
  //   delete &(pred[i]);
  //   delete &(errors[i]);
  // }
}
