#include "perceptron.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>

using namespace std;

pair<vector<vector<double> >&, vector<bool>&> read_points(const char* fname);

int main() {
  auto points = read_points("points.txt");
  vector<vector<double> >& X_train = points.first;
  vector<bool>& y_train = points.second;

  vector<double> weights0({0., 0., 0.});
  perceptron p(2, weights0);

  cout << "weights0: " << endl;
  p.print_weights();
  cout << "================" << endl;

  unsigned n_iters = p.fit(X_train, y_train, 1000);
  vector<bool> y_pred = p.predict(X_train);

  cout << "weights after fit: " << endl;
  p.print_weights();
  cout << "================" << endl;
  cout << "n_iters: " << n_iters << endl;
  cout << "================" << endl;
  for (int i = 0; i < y_train.size(); i++) {
    cout << "p" << i << ": y_true" << y_train[i] << "| y_pred" << y_pred[i] << endl;
  }

  delete &points;
  return 0;
}

pair<vector<vector<double> >&, vector<bool>&> read_points(const char* fname) {
  ifstream  f;
  f.open("points.txt");
  size_t N = 0;
  f >> N;
  // cout << "N is: " << N << endl;
  vector<vector<double> >& ret_X = *(new vector<vector<double> >(N));
  vector<bool>& ret_y = *(new vector<bool>);

  for (size_t i = 0; i < N; i++) {
    double x_curr = 0;
    double y_curr = 0;
    bool t_curr = false;
    f >> x_curr >> y_curr >> t_curr;
    ret_X[i].push_back(x_curr);
    ret_X[i].push_back(y_curr);
    ret_y.push_back(t_curr);
  }
  f.close();
  return *(new pair<vector<vector<double> >&, vector<bool>&>(ret_X, ret_y));
}
