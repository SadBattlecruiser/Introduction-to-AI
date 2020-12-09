
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


#define cimg_use_jpeg
using namespace cimg_library;

std::vector<CImg<unsigned char> >& load_images(const char* dir, unsigned num);
matrix<double>& img_to_matrix(CImg<unsigned char>& img, size_t size=256);
CImg<unsigned char>& matrix_to_img(matrix<double>& matrix, size_t size=256);

int main() {
  // CImg<unsigned char> img(640,400,1,3);
  // img.fill(0);
  //CImg<unsigned char> img(256,256);

  auto img_vec = load_images("img\\", 2);
  auto img = img_vec[0];
  matrix<double>& matr_img = img_to_matrix(img);
  img = matrix_to_img(matr_img);
  //img.display("1");

  network n(256*256, 16*16, 256*256);
  //network n(16*16, 16*16, 16*16);
  std::cout << "network done" << std::endl;
  matrix<double>& vmatr_img = matr_img.reshape(matr_img.get_n()*matr_img.get_m(), 1);
  std::cout << "reshape1 done" << std::endl;
  matrix<double>& vmatr_img_o = n.out(vmatr_img);
  std::cout << "out done" << std::endl;
  matrix<double>& matr_img_o = vmatr_img_o.reshape(matr_img.get_n(), matr_img.get_m());
  //matr_img_o.print();
  std::cout << "reshape2 done" << std::endl;
  auto img_o = matrix_to_img(matr_img_o);
  img_o.display("2");

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
