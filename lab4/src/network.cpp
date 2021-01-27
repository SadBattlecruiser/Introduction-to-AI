#include "network.hpp"

double mse(const matrix<double> v1, const matrix<double> v2) {
  double sum = 0;
  unsigned n = v1.get_n();
  for (unsigned i = 0; i < n; i++) {
    sum += v1.ij(i, 0) - v2.ij(i, 0);
  }
  return sum / n;
}

double sum(const matrix<double> v) {
  double sum = 0;
  unsigned n = v.get_n();
  for (unsigned i = 0; i < n; i++) {
    sum += v.ij(i, 0);
  }
  return sum;
}

matrix<double> pow2(matrix<double> v) {
  unsigned n = v.get_n();
  matrix<double> ret(n, 1);
  for (unsigned i = 0; i < n; i++) {
    ret[i][0] = v[i][0] * v[i][0];
  }
  return ret;
}

network::network(unsigned size0, unsigned size1, unsigned size2)
  : W1_(size1, size0), W2_(size2, size1) {
  // Заполняем матрицы
  double a,b,c;
  //a = 0.000001;
  //b = 0.0001;
  a = 0.01;
  b = 0.7 / sqrt(size1 * size0);
  //W1_.fill(0.000001);
  for (unsigned i = 0; i < size1; i++) {
  	for (unsigned j = 0; j < size0; j++) {
  		W1_[i][j] = (double)rand()/(double) RAND_MAX*(b-a)+a;
  	}
  }
  //W2_.fill(0.000001);
  for (unsigned i = 0; i < size2; i++) {
  	for (unsigned j = 0; j < size1; j++) {
  		W2_[i][j] = (double)rand()/(double) RAND_MAX*(b-a)+a;
  	}
  }

  //std::cout << "filling2" << std::endl;
  size0_ = size0;
  size1_ = size1;
  size2_ = size2;
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

unsigned network::fit(const vector< matrix<double>> orig, unsigned n_iters) {
  // Коэффициент обучения
  double l_c = 0.0000003;
  // Проходы
  double mse1 = 0, mse2 = 0;
  double mse2_prev = 10000000;
  for (unsigned k = 0; k < n_iters; k++) {
  	vector<matrix<double> > step_k;
    // Проходимся по всем кусочкам и суммируем ошибку
    int counter = 0;
    for (auto input : orig) {
      // Выход первого слоя
      matrix<double>& out1_temp = W1_ * input;
      // Выход второго слоя
      matrix<double>& out2_temp = W2_ * out1_temp;
      // Сохраняем в текущий шаг обучения
      step_k.push_back(out2_temp);
      // Ошибки на выходе второго слоя
      matrix<double>& errors2 = out2_temp - input;
      // Прогоняем в обратную сторону
      matrix<double>& W2_transp = W2_.transpose();
      matrix<double>& errors1 = W2_transp * errors2;
      // Корректируем веса
      // Второй слой
      for (unsigned i = 0; i < W2_.get_n(); i++) {
        for (unsigned l = 0; l < W2_.get_m(); l++) {
          W2_[i][l] -= l_c * errors2[i][0] * out1_temp[l][0];
        }
      }
      // Первый слой
      for (unsigned l = 0; l < W1_.get_n(); l++) {
        for (unsigned j = 0; j < W1_.get_m(); j++) {
          W1_[l][j] -= l_c * errors1[l][0] * input.ij(j, 0);
        }
      }

      if (counter == 0) {
      	mse1 = sum(pow2(errors1))/errors1.get_n();
      	mse2 = sum(pow2(errors2))/errors2.get_n();
        /*std::cout << "  =============" << std::endl;
        std::cout << "  iteration: " << k << std::endl;
        cout << "  mean square error2: " << mse2 << endl;
        cout << "  mean square error1: " << mse1 << endl;*/
      	if (abs(mse2 - mse2_prev) < 0.1) {
				  delete &out1_temp;
				  delete &out2_temp;
				  delete &W2_transp;
				  delete &errors1;
				  delete &errors2;
      		return k;
      	}
      	mse2_prev = mse2;
      }
      delete &out1_temp;
      delete &out2_temp;
      delete &W2_transp;
      delete &errors1;
      delete &errors2;
      counter++;

    }
    fit_steps.push_back(step_k);
  }
}
