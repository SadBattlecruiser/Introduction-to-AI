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
  std::cout << "network constructor begin" << std::endl;
  W1_.fill(0.0000001);
  W2_.fill(0.0000001);
  std::cout << "filling" << std::endl;
  size0_ = size0;
  size1_ = size1;
  size2_ = size2;
  std::cout << "network constructor end" << std::endl;
}

matrix<double>& network::out(const matrix<double>& in) const {
  return W2_ * (W1_ * in);
}

void network::fit(const std::vector< matrix<double>> orig) {
  size_t n = orig.size();
  // Количество проходов
  for (int k = 0; k < 3; k++) {
    // На каком элементе сейчас учимся
    size_t idx = 0;
    std::cout << "  fit k: " << k << std::endl;
    std::cout << "    idx: " << idx << std::endl;
    // Выход первого слоя
    matrix<double>& out1 = W1_ * orig[idx];
    // Выход второго слоя
    matrix<double>& out2 = W2_ * out1;
    // Ошибки на выходе второго слоя
    matrix<double>& errors2 = orig[idx] - out2;
    cout << "  err2_n " << errors2.get_n() << " err2_m " << errors2.get_m() << endl;
    cout << "  mean square error2: " << sum(pow2(errors2))/errors2.get_n() << endl;
    // Прогоняем в обратную сторону
    matrix<double>& W2_transp = W2_.transpose();
    matrix<double>& errors1 = W2_transp * errors2;
    cout << "  err1_n " << errors1.get_n() << " err1_m " << errors1.get_m() << endl;
    cout << "  mean square error1: " << sum(pow2(errors1))/errors1.get_n() << endl;
    //errors2.print();
    // Корректируем веса
    double l_c = 0.0005;   // Коэффициент обучения
    for (size_t i = 0; i < W2_.get_n(); i++) {
      for (size_t j = 0; j < W2_.get_m(); j++) {
        //W2_[i][j] += -l_c * errors2[i][0] * out1[j][0];
        W2_[i][j] += -l_c * errors2[i][0];
      }
    }
    for (size_t i = 0; i < W1_.get_n(); i++) {
      for (size_t j = 0; j < W1_.get_m(); j++) {
        //W1_[i][j] += -l_c * errors1[i][0] * orig[idx].ij(j, 0);
        W1_[i][j] += -l_c * errors1[i][0];
      }
    }
    std::cout << "corrected weigths" << std::endl;

    delete &out1;
    delete &out2;
    delete &W2_transp;
    delete &errors1;
    delete &errors2;
  }
}
