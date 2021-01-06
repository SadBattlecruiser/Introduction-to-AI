#include "network.hpp"

double mse(const matrix<double> v1, const matrix<double> v2) {
  double sum = 0;
  size_t n = v1.get_n();
  for (size_t i = 0; i < n; i++) {
    sum += v1.ij(i, 0) - v2.ij(i, 0);
  }
  return sum / n;
}

double sum(const matrix<double> v) {
  double sum = 0;
  size_t n = v.get_n();
  for (size_t i = 0; i < n; i++) {
    sum += v.ij(i, 0);
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
  W1_.fill(0.000001);
  //std::cout << "filling1" << std::endl;
  W2_.fill(0.000001);
  //std::cout << "filling2" << std::endl;
  size0_ = size0;
  size1_ = size1;
  size2_ = size2;
  std::cout << "network constructor end" << std::endl;
}

matrix<double>& network::out(const matrix<double>& in) const {
  return W2_ * (W1_ * in);
}

vector<vector<matrix<double> > >& network::out(const vector<vector<matrix<double> > >& in) const {
  vector<vector<matrix<double> > >& ret_vec = *(new vector<vector<matrix<double> > >);
  for (auto matr_i : in) {
    vector<matrix<double> > cells_out;
    for (auto cell_i : matr_i) {
      matrix<double>& cell_out = this->out(cell_i);
      cells_out.push_back(cell_out);
      delete &cell_out;
    }
    ret_vec.push_back(cells_out);
  }
  return ret_vec;
}

void network::fit(const vector< matrix<double>> orig) {
  // Количество проходов
  for (int k = 0; k < 5; k++) {
    // Проходимся по всем кусочкам и суммируем ошибку
    int counter = 0;
    for (auto input : orig) {
      // Выход первого слоя
      matrix<double>& out1_temp = W1_ * input;
      // Выход второго слоя
      matrix<double>& out2_temp = W2_ * out1_temp;
      // Ошибки на выходе второго слоя
      matrix<double>& errors2 = out2_temp - input;
      // Прогоняем в обратную сторону
      matrix<double>& W2_transp = W2_.transpose();
      matrix<double>& errors1 = W2_transp * errors2;
      // Корректируем веса
      double l_c = 1;   // Коэффициент обучения
      // Второй слой
      for (size_t i = 0; i < W2_.get_n(); i++) {
        for (size_t l = 0; l < W2_.get_m(); l++) {
          W2_[i][l] += -l_c * errors2[i][0] * out1_temp[l][0];
        }
      }
      // // Первый слой
      // for (size_t l = 0; l < W1_.get_n(); l++) {
      //   for (size_t j = 0; j < W1_.get_m(); j++) {
      //     W1_[l][j] += (l_c*0.001) * errors1[l][0] * input.ij(j, 0);
      //   }
      // }

      // if (counter == 0) {
      //   std::cout << "iteration: " << counter << std::endl;
      //   std::cout << "W1_.n " << W1_.get_n() << " W1_._m " << W1_.get_m() << std::endl;
      //   std::cout << "errors1.n " << errors1.get_n() << " errors1._m " << errors1.get_m() << std::endl;
      //   std::cout << "input.n " << input.get_n() << " input._m " << input.get_m() << std::endl;
      //   std::cout << "errors2.n " << errors2.get_n() << " errors2._m " << errors2.get_m() << std::endl;
      //   cout << "  mean square error2: " << sum(pow2(errors2))/errors2.get_n() << endl;
      //   cout << "  mean square error1: " << sum(pow2(errors1))/errors1.get_n() << endl;
      //   std::cout << "=============" << std::endl;
      // }

      delete &out1_temp;
      delete &out2_temp;
      delete &W2_transp;
      delete &errors1;
      delete &errors2;
      counter++;
    }
  }
}
