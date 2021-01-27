#ifndef NETWORK
#define NETWORK

#include <vector>
#include <iostream>
#include <cmath>

#include "matrix.cpp"

using namespace std;

class network{
public:
  // Кол-во входов, кол-во нейронов первого слоя, кол-во нейроно второго слоя
  network(unsigned size0, unsigned size1, unsigned size2);
  matrix<double>& out(const matrix<double>& in) const;
  vector<vector<matrix<double> > >& out(const vector<vector<matrix<double> > >& in) const;
  unsigned fit(const vector< matrix<double> > X_train, unsigned n_iters = 5);
  
  unsigned size0_;
  unsigned size1_;
  unsigned size2_;
  matrix<double> W1_;
  matrix<double> W2_;
  vector<vector<matrix<double> > > fit_steps;
private:
};

#endif
