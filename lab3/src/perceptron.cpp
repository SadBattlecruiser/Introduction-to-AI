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
                             const std::vector<bool>& y_train, size_t idx, unsigned max_iter) {
  unsigned count = 0;
  double f_coeff = 0.2;
  double y_true = y_train[idx];
  while (count < max_iter) {
    double y_pred = this->out(X_train[idx]);
    if (y_true == y_pred) {
      //std::cout << "y_true == y_pred" << std::endl;
      break;
    }
    // Отдельно поляризацию
    weights_[0] += f_coeff * (y_true - y_pred);
    // weights_[1] += f_coeff * (y_true - y_pred) * X_train[idx][0];
    // weights_[2] += f_coeff * (y_true - y_pred) * X_train[idx][1];
    // И остальное
    for (size_t i = 1; i < N_inputs_; i++) {
      weights_[i] += f_coeff * (y_true - y_pred) * X_train[idx][i - 1];
    }
    count++;
  }
  return count;
};

unsigned perceptron::fit(const std::vector<std::vector<double> >& X_train,
                         const std::vector<bool>& y_train, unsigned max_iter) {
  unsigned total_count = 0;
  for (unsigned iter = 0; iter < max_iter; iter++) {
    for (size_t i = 0; i < X_train.size(); i++) {
      total_count += this->fit_one(X_train, y_train, i, 10);
    }
  }
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
