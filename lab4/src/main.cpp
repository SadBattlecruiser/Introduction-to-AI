
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
//#include <stdio.h>
//#include <typeinfo>
//#include "windows.h"
#include "CImg.h"

#include "matrix.cpp"
#include "network.hpp"


//#define cimg_use_jpeg
using namespace cimg_library;

std::vector<CImg<unsigned char> >& load_images(const char* dir, unsigned num);

matrix<double>& img_to_matrix(CImg<unsigned char>& img, size_t size=256);
CImg<unsigned char>& matrix_to_img(matrix<double>& matrix, size_t size=256);

vector<matrix<double> >& images_to_matrix(std::vector<CImg<unsigned char> >& images, size_t size=256);

int main() {
  // matrix<double> A(3,3);
  // matrix<double> B(3,3);
  // for (int i = 0; i < 3; i++) {
  //   for (int j = 0; j < 3; j++) {
  //     A[i][j] = 10*i + j;
  //     B[i][j] = 3*j + 2*i;
  //   }
  // }
  // A.print();
  // B.print();
  // (A*B).print();
  // matrix<double> D(3,2);
  // D.fill(4.);
  // D[0][0] = 1;
  // D[1][0] = 2;
  // D[2][0] = 3;
  // matrix<double> E = D.transpose();
  // E.print();
  // E.transpose().print();
  // (A+B).print();
  // (A+=B).print();
  // (A*2).print();
  // (A.reshape(9,1)).print();

  // CImg<unsigned char> img(640,400,1,3);
  // img.fill(0);
  // CImg<unsigned char> img(256,256);

  auto img_vec = load_images("img\\", 2);
  std::cout << "load_images done" << std::endl;
  auto img_m_vec = images_to_matrix(img_vec);
  std::cout << "images_to_matrix done" << std::endl;
  // Переразбиваем матрицы в одномерные вектора
  vector<matrix<double> > img_rm_vec;
  for (auto img : img_m_vec) {
    auto temp = img.reshape(256*256, 1);
    // std::cout << img.get_n() << std::endl;
    // std::cout << img.get_m() << std::endl;
    img_rm_vec.push_back(temp);
  }
  std::cout << "reshape done" << std::endl;
  // Сама сеть
  network n(256*256, 256*256, 256*256);
  //network n(32*32, 32*32, 32*32);
  std::cout << "network create" << std::endl;
  std::cout << "W1.n: " << n.W1_.get_n() << std::endl;
  std::cout << "W1.m: " << n.W1_.get_m() << std::endl;
  std::cout << "W2.n: " << n.W2_.get_n() << std::endl;
  std::cout << "W2.m: " << n.W2_.get_m() << std::endl;
  // Обучаемся
  n.W1_.fill(0.);
  n.W2_.fill(0.);
  for (size_t i = 0; i < 256*256; i++) {
    n.W1_[i][i] = 1.;
    n.W2_[i][i] = 1.;
  }
  //n.fit(img_rm_vec);
  std::cout << "fit done" << std::endl;
  // Смотрим пробную картинку
  matrix<double> test_rm = n.out(img_rm_vec[0]);
  matrix<double> test_m = test_rm.reshape(256, 256);
  // test_m.print();
  auto test = matrix_to_img(test_m);
  test.display("2");


  // network n(16, 16, 16);
  // n.W1_.fill(0.);
  // n.W2_.fill(0.);
  // for (size_t i = 0; i < 16; i++) {
  //   n.W1_[i][i] = 1.;
  //   n.W2_[i][i] = 1.;
  // }
  // std::cout << "network create" << std::endl;
  // std::cout << "W1.n: " << n.W1_.get_n() << std::endl;
  // std::cout << "W1.m: " << n.W1_.get_m() << std::endl;
  // std::cout << "W2.n: " << n.W2_.get_n() << std::endl;
  // std::cout << "W2.m: " << n.W2_.get_m() << std::endl;
  // // Тест
  // matrix<double> testvec(16, 1);
  // for (int i = 0; i < 16; i++) {
  //   testvec[i][0] = i;
  // }
  // n.out(testvec).print();


  return 0;
}

std::vector<CImg<unsigned char> >& load_images(const char* dir, unsigned num) {
  std::string str(dir);
  std::vector<CImg<unsigned char> >& ret = *(new std::vector<CImg<unsigned char> >);
  for (size_t i = 0; i < num; i++) {
    std::string name = str + std::to_string(i) + "-in.bmp";
    CImg<unsigned char> img(name.c_str());
    ret.push_back(img);
  }
  return ret;
}

matrix<double>& img_to_matrix(CImg<unsigned char>& img, size_t size) {
  matrix<double>& ret = *(new matrix<double>(size, size));
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      ret[i][j] = img(i,j);
    }
  }
  return ret;
}

CImg<unsigned char>& matrix_to_img(matrix<double>& matrix, size_t size) {
  CImg<unsigned char>& ret = *(new CImg<unsigned char>(size, size));
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size; j++) {
      //ret[i][j] = img(i,j);
      ret(i,j) = matrix[i][j];
    }
  }
  return ret;
}

vector<matrix<double> >& images_to_matrix(std::vector<CImg<unsigned char> >& images, size_t size) {
  vector<matrix<double> >& ret_vec = *(new vector<matrix<double> >);
  for (auto img : images) {
    // auto temp = img_to_matrix(img, size);
    // temp.print();
    ret_vec.push_back(img_to_matrix(img, size));
  }
  // ret_vec[0].print();
  return ret_vec;
}
