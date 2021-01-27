#include "perceptron.hpp"

perceptron::perceptron(size_t N_features, const std::vector<double>& weights0, double activation_trs) {
  N_inputs_ = N_features + 1;               // Есть еще поляризационная единичка
  weights_ = weights0;
  activation_trs_ = activation_trs;
}

double perceptron::weighted_sum(const std::vector<double>& input) const {
  double sum = weights_[0];                 // Вклад поляризации
  for (size_t i = 1; i < N_inputs_; i++) {
    sum += input[i-1] * weights_[i];
  }
  return sum;
};

double perceptron::out(const std::vector<double>& input) const {
  double sum = this->weighted_sum(input);
  if (sum >= activation_trs_) {
    return 1.;
  }
  else {
    return 0.;
  }
};

unsigned perceptron::fit_one(const std::vector<std::vector<double> >& X_train,
                             const std::vector<bool>& y_train, size_t idx, std::ofstream& file, unsigned max_iter) {
  unsigned count = 0;
  double f_coeff = 4;         // Коэффициент обучения
  double y_true = y_train[idx];
  while (count < max_iter) {
    double y_pred = this->out(X_train[idx]);
    if (y_true == y_pred) {
      break;
    }
    // Отдельно поляризацию
    weights_[0] += f_coeff * (y_true - y_pred);
    // И остальное
    for (size_t i = 1; i < N_inputs_; i++) {
      weights_[i] += f_coeff * (y_true - y_pred) * X_train[idx][i - 1];
    }
    //std::cout <<"-----" << std::endl;
    //std::cout << weights_[0] << ' ' << weights_[1] << ' ' << weights_[2] << ' ' << std::endl;
    //std::cout <<"-----" << std::endl;
    count++;
    // Выводим в файл точки разделяющей линии
    double x1 = -20;
    double y1 = -weights_[0]/weights_[2] - weights_[1]/weights_[2]*x1;
    double x2 = 20;
    double y2 = -weights_[0]/weights_[2] - weights_[1]/weights_[2]*x2;
		if (y1 > 20) {
			x1 = x1 * 20./y1;
			y1 = 20.;  	
		}
		if (y1 < -20) {
			x1 = x1 * (-20./y1);
			y1 = -20.;  	
		}
		if (y2 > 20) {
			x2 = x2 * 20./y2;
			y2 = 20.;  	
		}
		if (y2 < -20) {
			x2 = x2 * (-20./y2);
			y2 = -20.;  	
		}
    file << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << std::endl;

  }
  return count;
};

unsigned perceptron::fit(const std::vector<std::vector<double> >& X_train,
                         const std::vector<bool>& y_train, unsigned max_iter) {
                         
  std::ofstream ofile("fit_lines.txt");
  // Выводим в файл точки начальной разделяющей линии
  double x1 = -20;
  double y1 = -weights_[0]/weights_[2] - weights_[1]/weights_[2]*x1;
  double x2 = 20;
  double y2 = -weights_[0]/weights_[2] - weights_[1]/weights_[2]*x2;
  if (y1 > 20) {
  	x1 = x1 * 20./y1;
  	y1 = 20.;  	
  }
  if (y1 < -20) {
  	x1 = x1 * (-20./y1);
  	y1 = -20.;  	
  }
  if (y2 > 20) {
  	x2 = x2 * 20./y2;
  	y2 = 20.;  	
  }
  if (y2 < -20) {
  	x2 = x2 * (-20./y2);
  	y2 = -20.;  	
  }
  ofile << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << std::endl;
  // Учимся
  unsigned total_count = 0;
  for (unsigned iter = 0; iter < max_iter; iter++) {
    for (size_t i = 0; i < X_train.size(); i++) {
      total_count += this->fit_one(X_train, y_train, i, ofile, 4);
    }
  }
  ofile.close();
  return total_count;
}

std::vector<bool> perceptron::predict(
  const std::vector<std::vector<double> >& X_train) const {
  std::vector<bool> y_pred(X_train.size());
  for (int i = 0; i < X_train.size(); i++) {
    y_pred[i] = this->out(X_train[i]);
  }
  return y_pred;
}

std::vector<double> perceptron::get_weights() const {
  return weights_;
}

void perceptron::print_weights() const {
  std::cout << "---------" << std::endl;
  for (int i = 0; i < N_inputs_; i++) {
    std::cout << "w" << i << " : " << weights_[i] << std::endl;
  }
}
