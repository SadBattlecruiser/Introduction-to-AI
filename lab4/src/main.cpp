
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <stdlib.h>
#include <stdio.h>
#include <png.h>

#include "matrix.cpp"
#include "network.hpp"

struct png_img {
	unsigned width, height;
	png_bytep* row_pointers = NULL;
	int bit_depth, color_type;
	int interlace_type;
  int compression_type;
  int filter_method;
};

png_img read_png_file(const char *filename);

void write_png_file(const char *filename, png_img img);

std::vector<png_img>& load_images(const char* dir, unsigned num);

matrix<double>& img_to_matrix(png_img& img, unsigned size=256);

vector<matrix<double> >& images_to_matrices(vector<png_img>& images, unsigned side=256);

vector<vector<matrix<double> > >& matrices_to_cell_vecs(vector<matrix<double> >& matrices,
                                                        unsigned Nh=2, unsigned Nv=2, unsigned side=256);

vector<matrix<double> >& cell_vecs_to_matrices(vector<vector<matrix<double> > >& cell_vecs,
                                               unsigned Nh=2, unsigned Nv=2, unsigned side=256);

png_img& matrix_to_img(matrix<double>& matrix, unsigned size=256);

vector<png_img>& matrices_to_images(vector<matrix<double> >& matrices, unsigned side=256);

void write_images(vector<png_img>& images, const char* dir);

//=====================================================================================================
unsigned SIDE = 256;
unsigned Nh = 2;
unsigned Nv = 2;
unsigned L = 1;

int main(int argc, char* argv[]) {
	// Если передали размер ячейки
	if (argc == 4) {
		Nh = stoi(argv[1]);
		Nv = stoi(argv[2]);
		L = stoi(argv[3]);
	}
  std::cout << "Nh = " << Nh <<" | Nv = " << Nv << " | L = " << L << std::endl;
  // Загружаем изображения с диска
  auto img_vec = load_images("./img_in/", 4);
  std::cout << "load_images done" << std::endl;
  // Превращаем изображения в матрицы
  auto img_m_vec = images_to_matrices(img_vec, SIDE);
  std::cout << "images_to_matrices done" << std::endl;
  // Переразбиваем каждую матрицу в несколько одномерных векторов
  auto img_cv_vec = matrices_to_cell_vecs(img_m_vec, Nh, Nv, SIDE);
  std::cout << "matrices_to_cell_vecs done" << std::endl;
  // Сама сеть
  network n(Nh*Nv, L, Nh*Nv);
  std::cout << "network create" << std::endl;
  std::cout << "W1.n: " << n.W1_.get_n() << std::endl;
  std::cout << "W1.m: " << n.W1_.get_m() << std::endl;
  std::cout << "W2.n: " << n.W2_.get_n() << std::endl;
  std::cout << "W2.m: " << n.W2_.get_m() << std::endl;
  // Обучаемся по первой картинке
  unsigned n_iters_max = 	40;
  unsigned n_iters = n.fit(img_cv_vec[0], n_iters_max)
  // Прогоняем всё через сеть
  auto img_cv_vec_out = n.out(img_cv_vec);
  std::cout << "n.out done" << std::endl;
  // Одномерные вектора обратно в матрицы
  auto img_m_vec_out = cell_vecs_to_matrices(img_cv_vec_out, Nh, Nv, SIDE);
  std::cout << "cell_vecs_to_matrices done" << std::endl;
  // Конвертим обратно в изображения
  auto img_vec_out = matrices_to_images(img_m_vec_out, SIDE);
  std::cout << "matrices_to_images done" << std::endl;
  // Записываем в файлы
  write_images(img_vec_out, "./img_out/");
  std::cout << "write out images done" << std::endl;
  // Получаем картинки для шагов обучения
  auto fit_steps_m_vec = cell_vecs_to_matrices(n.fit_steps, Nh, Nv, SIDE);
  auto fit_steps_vec = matrices_to_images(fit_steps_m_vec, SIDE);
  write_images(fit_steps_vec, "./fit_steps/");

  std::cout << "write fit steps done" << std::endl;
  std::cout << "==============" << std::endl;
  std::cout << "n_iterations: " << n_iters << std::endl;
  return 0;
}

//=====================================================================================================


std::vector<png_img>& load_images(const char* dir, unsigned num) {
  std::string str(dir);
  std::vector<png_img>& ret = *(new std::vector<png_img>);
  for (unsigned i = 0; i < num; i++) {
    std::string name = str + std::to_string(i) + "-in.png";
    png_img img = read_png_file(name.c_str());
    //std::cout << "read DONE" << std::endl;
    ret.push_back(img);
  }
  return ret;
}

matrix<double>& img_to_matrix(png_img& img, unsigned size) {
  matrix<double>& ret = *(new matrix<double>(size, size));
  for (unsigned i = 0; i < size; i++) {
    for (unsigned j = 0; j < size; j++) {
      ret[i][j] = img.row_pointers[i][j];
    }
  }
  return ret;
}

vector<matrix<double> >& images_to_matrices(std::vector<png_img>& images, unsigned side) {
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
                                                    unsigned Nh, unsigned Nv, unsigned side) {
  vector<vector<matrix<double> > >& ret_vec = *(new vector<vector<matrix<double> > >);
  for (auto matr_i : matrices) {
    vector<matrix<double> > cells_i;
    for (unsigned i = 0; i < side / Nh; i++) {
      for (unsigned j = 0; j < side / Nv; j++) {
        unsigned slice_n_beg = Nh * i;
        unsigned slice_n_end = Nh * (i + 1);
        unsigned slice_m_beg = Nv * j;
        unsigned slice_m_end = Nv * (j + 1);
        matrix<double>& cell_i = matr_i.slice(slice_n_beg, slice_n_end,
                                            slice_m_beg, slice_m_end);
        matrix<double>& cell_vec = cell_i.reshape(Nh*Nv, 1);
        cells_i.push_back(cell_vec);
        //delete &cell;
        //delete &cell_vec;
      }
    }
    ret_vec.push_back(cells_i);
  }
  return ret_vec;
}

//////////

vector<matrix<double> >& cell_vecs_to_matrices(vector<vector<matrix<double> > >& cell_vecs,
                                               unsigned Nh, unsigned Nv, unsigned side) {
  vector<matrix<double> >& ret_matrices = *(new vector<matrix<double> >);
  for (auto cell_vecs_i : cell_vecs) {
    matrix<double> matr_i(side, side);
    for (unsigned i = 0; i < side / Nh; i++) {
      for (unsigned j = 0; j < side / Nv; j++) {
        unsigned slice_n_beg = Nh * i;
        unsigned slice_m_beg = Nv * j;
        matrix<double>& cell_i = cell_vecs_i[i * side / Nv + j].reshape(Nh, Nv);
        for (unsigned k = 0; k < Nh; k++) {
          for (unsigned l = 0; l < Nv; l++) {
            matr_i[slice_n_beg + k][slice_m_beg + l] = cell_i[k][l];
          }
        }
        //delete &cell_i;
      }
    }
    ret_matrices.push_back(matr_i);
  }
  return ret_matrices;
}

png_img& matrix_to_img(matrix<double>& matrix, unsigned size) {
  png_img& ret = *(new png_img);
  ret.color_type = 0;
  ret.bit_depth = 8;
  ret.width = SIDE;
  ret.height = SIDE;
  ret.interlace_type = PNG_INTERLACE_NONE;
  ret.compression_type = PNG_COMPRESSION_TYPE_DEFAULT;
  ret.filter_method = PNG_FILTER_TYPE_DEFAULT;
  // memory allocation
  ret.row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * ret.height);
  for(int y = 0; y < ret.height; y++) {
    ret.row_pointers[y] = (png_byte*)malloc(256);
  }

  for (unsigned i = 0; i < size; i++) {
    for (unsigned j = 0; j < size; j++) {
      ret.row_pointers[i][j] = matrix[i][j];
    }
  }
  return ret;
}

vector<png_img>& matrices_to_images(vector<matrix<double> >& matrices, unsigned side) {
  vector<png_img>& ret_images = *(new vector<png_img>);
  for (auto matr_i : matrices) {
    png_img& img_i = matrix_to_img(matr_i);
    ret_images.push_back(img_i);
    //delete &img_i;
  }
  return ret_images;
}

void write_images(vector<png_img>& images, const char* dir) {
	for (unsigned i = 0; i < images.size(); i++) {
		std::string file_name = string(dir) + to_string(i) + ".png";
		//images[i].normalize(0, 255);
		//images[i].save(file_name.c_str());
		write_png_file(file_name.c_str(), images[i]);
  }
}

///////////////////////////////////////////////////////////

png_img read_png_file(const char *filename) {
	png_img img;

  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png) abort();

  png_infop info = png_create_info_struct(png);
  if(!info) abort();

  if(setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  img.width      = png_get_image_width(png, info);
  img.height     = png_get_image_height(png, info);
  img.color_type = png_get_color_type(png, info);
  img.bit_depth  = png_get_bit_depth(png, info);

  png_get_IHDR(png, info, &img.width, &img.height, &img.bit_depth,
  	&img.color_type, &img.interlace_type, &img.compression_type, &img.filter_method);

  //std::cout << "==================" << std::endl;
  //printf ("Ширина = %d, высота = %d\n", img.width, img.height);
  //printf ("Тип цвета = %d, глубина цвета = %d\n", img.color_type, img.bit_depth);
  //printf ("Количество байт в строке = %d\n", png_get_rowbytes(png, info));
  //std::cout << "==================" << std::endl;

  if(img.bit_depth == 16)
  	//std::cout << "WARNING: img.bit_depth == 16" << std::endl;
    png_set_strip_16(png);

  if(img.color_type == PNG_COLOR_TYPE_PALETTE) {
  	//std::cout << "img.color_type == PNG_COLOR_TYPE_PALETTE" << std::endl;
    png_set_palette_to_rgb(png);
    png_set_rgb_to_gray(png, 1, NULL, NULL);
  	}

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if(img.color_type == PNG_COLOR_TYPE_GRAY && img.bit_depth < 8) {
  	//std::cout << "img.color_type == PNG_COLOR_TYPE_GRAY && img.bit_depth < 8" << std::endl;
    png_set_expand_gray_1_2_4_to_8(png);
    }

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if(img.color_type == PNG_COLOR_TYPE_RGB ||
     img.color_type == PNG_COLOR_TYPE_GRAY ||
     img.color_type == PNG_COLOR_TYPE_PALETTE) {
  	//std::cout << "img.color_type == PNG_COLOR_TYPE_RGB || img.color_type == PNG_COLOR_TYPE_GRAY || img.color_type == PNG_COLOR_TYPE_PALETTE" << std::endl;
    //png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
  }

  //if(img.color_type == PNG_COLOR_TYPE_GRAY ||
  //   img.color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
  //  png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  /*std::cout << "==================" << std::endl;
  std::cout << "После преобразований: " << std::endl;
  printf ("Ширина = %d, высота = %d\n", img.width, img.height);
  printf ("Тип цвета = %d, глубина цвета = %d\n", img.color_type, img.bit_depth);
  printf ("Количество байт в строке = %d\n", png_get_rowbytes(png, info));
  std::cout << (img.interlace_type == PNG_INTERLACE_NONE) << std::endl;
  std::cout << (img.compression_type == PNG_COMPRESSION_TYPE_DEFAULT) << std::endl;
  std::cout << (img.filter_method == PNG_FILTER_TYPE_DEFAULT) << std::endl;
  std::cout << "==================" << std::endl;*/

  if (img.row_pointers) abort();

  img.row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * img.height);
  for(int y = 0; y < img.height; y++) {
    img.row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }

  png_read_image(png, img.row_pointers);

  fclose(fp);


  png_destroy_read_struct(&png, &info, NULL);

  return img;
}

//////////////////////////////

void write_png_file(const char *filename, png_img img) {
  //int y;

  FILE *fp = fopen(filename, "w");
  if(!fp) abort();

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  png_infop info = png_create_info_struct(png);
  if (!info) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    img.width, img.height,
    img.bit_depth,
    //PNG_COLOR_TYPE_RGBA,
    PNG_COLOR_TYPE_GRAY,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );

  png_set_IHDR(png, info, img.width, img.height, img.bit_depth,
	img.color_type, img.interlace_type, img.compression_type, img.filter_method);

  png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  if (!img.row_pointers) abort();

  png_write_image(png, img.row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < img.height; y++) {
    free(img.row_pointers[y]);
  }
  free(img.row_pointers);

  fclose(fp);

  png_destroy_write_struct(&png, &info);
}
