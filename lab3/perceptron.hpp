#ifndef PERCEPTRON_HPP
#define PERCEPTRON_HPP

#include <iostream>
#include <fstream>
#include <vector>

class perceptron {
  public:
    perceptron(size_t N_features, const std::vector<double>& weights0,
               double activation_trs = 0.0);
    double weighted_sum(const std::vector<double>& input) const;        // Посчитать взвешенную сумму входа
    double out(const std::vector<double>& input) const;                 // Сумма + ф. активации
    unsigned fit_one(const std::vector<std::vector<double> >& X_train,  // Обучится на одном примере
                     const std::vector<bool>& y_train, size_t idx,
					 std::ofstream& file,
                     unsigned max_iter = 10);
    unsigned fit(const std::vector<std::vector<double> >& X_train,      // Полностью обучиться на выборке
                 const std::vector<bool>& y_train, unsigned max_iter=1000);
    std::vector<bool> predict(const std::vector<std::vector<double> >& X_train) const;
    std::vector<double> get_weights() const;
    void print_weights() const;
  private:
    size_t N_inputs_;
    std::vector<double> weights_;
    double activation_trs_;
};

#endif
