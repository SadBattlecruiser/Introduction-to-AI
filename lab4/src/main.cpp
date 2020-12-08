
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>

#include "matrix.cpp"
#include "windows.h"
#include "CImg.h"

using namespace cimg_library;

int main() {

  matrix<double> A(4,2);
  A.fill(4);
  matrix<double> B(2,3);
  B.fill(2);
  auto C = A*B;
  //C.print();

  CImg<unsigned char> img(640,400,1,3);
  img.fill(0);
  unsigned char purple[] = { 255,0,255 };
  img.draw_text(100,100,"Hello World",purple);
  img.display("My first CImg code");

  return 0;
}
