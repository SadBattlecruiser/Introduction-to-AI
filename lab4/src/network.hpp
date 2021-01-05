#ifndef NETWORK
#define NETWORK

#include <vector>
#include <iostream>

#include "matrix.cpp"

using namespace std;

class network{
public:
  // Кол-во входов, кол-во нейронов первого слоя, кол-во нейроно второго слоя
  network(size_t size0, size_t size1, size_t size2);
  matrix<double>& out(const matrix<double>& in) const;
  vector<vector<matrix<double> > >& out(const vector<vector<matrix<double> > >& in) const;
  void fit(const vector< matrix<double> > X_train);
  // cimg_library::CImg<unsigned char>& out(const cimg_library::CImg<unsigned char>& in) const;
  // matrix<double>& back(const matrix<double>& in) const;
  size_t size0_;
  size_t size1_;
  size_t size2_;
  matrix<double> W1_;
  matrix<double> W2_;
private:
};

#endif
