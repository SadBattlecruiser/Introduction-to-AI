
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

vector<matrix<double> >& images_to_matrices(vector<CImg<unsigned char> >& images, size_t side=256);
vector<vector<matrix<double> > >& matrices_to_cell_vecs(vector<matrix<double> >& matrices,
                                                        size_t cell=32, size_t side=256);

vector<matrix<double> >& cell_vecs_to_matrices(vector<vector<matrix<double> > >& cell_vecs,
                                               size_t cell=32, size_t side=256);
vector<CImg<unsigned char> >& matrices_to_images(vector<matrix<double> >& matrices, size_t side=256);

int main() {
  size_t SIDE = 256;
  size_t CELL = 2;

  // Загружаем изображения с диска
  auto img_vec = load_images("img\\", 2);
  std::cout << "load_images done" << std::endl;
  // Превращаем изображения в матрицы
  auto img_m_vec = images_to_matrices(img_vec, SIDE);
  std::cout << "images_to_matrix done" << std::endl;
  // Переразбиваем каждую матрицу в несколько одномерных векторов
  auto img_cv_vec = matrices_to_cell_vecs(img_m_vec, CELL, SIDE);
  std::cout << "reshape done" << std::endl;
  // Сама сеть
  //network n(SIDE*SIDE, SIDE*SIDE, SIDE*SIDE);
  //network n(CELL*CELL, CELL/2*CELL/2, CELL*CELL);
  //network n(CELL*CELL, CELL/2*CELL/2, CELL*CELL);
  network n(CELL*CELL, CELL*CELL, CELL*CELL);
  std::cout << "network create" << std::endl;
  std::cout << "W1.n: " << n.W1_.get_n() << std::endl;
  std::cout << "W1.m: " << n.W1_.get_m() << std::endl;
  std::cout << "W2.n: " << n.W2_.get_n() << std::endl;
  std::cout << "W2.m: " << n.W2_.get_m() << std::endl;
  // Обучаемся по первой картинке
  n.fit(img_cv_vec[0]);
  std::cout << "fit done" << std::endl;
  // Прогоняем всё через сеть
  auto img_cv_vec_out = n.out(img_cv_vec);
  std::cout << "n.out() done" << std::endl;
  // Одномерные вектора обратно в матрицы
  auto img_m_vec_out = cell_vecs_to_matrices(img_cv_vec_out, CELL, SIDE);
  std::cout << "reshape2 done" << std::endl;
  // Конвертим обратно в изображения
  auto img_vec_out = matrices_to_images(img_m_vec_out, SIDE);
  // Смотрим пробную картинку
  auto test = img_vec_out[0];
  test.display("2");

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

vector<matrix<double> >& images_to_matrices(std::vector<CImg<unsigned char> >& images, size_t side) {
  vector<matrix<double> >& ret_vec = *(new vector<matrix<double> >);
  for (auto img : images) {
    // auto temp = img_to_matrix(img, size);
    // temp.print();
    ret_vec.push_back(img_to_matrix(img, side));
  }
  // ret_vec[0].print();
  return ret_vec;
}

vector<vector<matrix<double> > >& matrices_to_cell_vecs(vector<matrix<double> >& matrices,
                                                    size_t cell, size_t side) {
  vector<vector<matrix<double> > >& ret_vec = *(new vector<vector<matrix<double> > >);
  for (auto matr_i : matrices) {
    vector<matrix<double> > cells_i;
    for (size_t i = 0; i < side / cell; i++) {
      for (size_t j = 0; j < side / cell; j++) {
        size_t slice_n_beg = cell * i;
        size_t slice_n_end = cell * (i + 1);
        size_t slice_m_beg = cell * j;
        size_t slice_m_end = cell * (j + 1);
        matrix<double>& cell_i = matr_i.slice(slice_n_beg, slice_n_end,
                                            slice_m_beg, slice_m_end);
        matrix<double>& cell_vec = cell_i.reshape(cell*cell, 1);
        cells_i.push_back(cell_vec);
        delete &cell;
        delete &cell_vec;
      }
    }
    ret_vec.push_back(cells_i);
  }
  return ret_vec;
}

vector<matrix<double> >& cell_vecs_to_matrices(vector<vector<matrix<double> > >& cell_vecs,
                                               size_t cell, size_t side) {
  vector<matrix<double> >& ret_matrices = *(new vector<matrix<double> >);
  for (auto cell_vecs_i : cell_vecs) {
    matrix<double> matr_i(side, side);
    for (size_t i = 0; i < side / cell; i++) {
      for (size_t j = 0; j < side / cell; j++) {
        size_t slice_n_beg = cell * i;
        size_t slice_m_beg = cell * j;
        matrix<double>& cell_i = cell_vecs_i[i * side / cell + j].reshape(cell, cell);
        for (size_t k = 0; k < cell; k++) {
          for (size_t l = 0; l < cell; l++) {
            matr_i[slice_n_beg + k][slice_m_beg + l] = cell_i[k][l];
          }
        }
        delete &cell_i;
      }
    }
    ret_matrices.push_back(matr_i);
  }
  return ret_matrices;
}

vector<CImg<unsigned char> >& matrices_to_images(vector<matrix<double> >& matrices, size_t side) {
  vector<CImg<unsigned char> >& ret_images = *(new vector<CImg<unsigned char> >);
  for (auto matr_i : matrices) {
    CImg<unsigned char>& img_i = matrix_to_img(matr_i);
    ret_images.push_back(img_i);
    delete &img_i;
  }
  return ret_images;
}
