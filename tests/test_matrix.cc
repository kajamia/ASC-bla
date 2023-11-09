#include <iostream>

#include "matrix.h"
#include "matrix_expression.h"
#include "vector.h"

namespace bla = ASC_bla;

// for testing move semantics
bla::Matrix<double, bla::RowMajor> returnsmatrix(size_t m, size_t n){
  bla::Matrix<double, bla::RowMajor> D(m, n);
  D(3, 3) = 5.6;
  return D;
}

// miscellaneous tests (storage, access, output, ...)
void misc_tests(){

  size_t m = 6;
  size_t n = 5;
  bla::Matrix<double, bla::RowMajor> A(m, n);
  bla::Matrix<double, bla::ColMajor> B(m, n);
  
  A = 3.14;
  A(2, 2) = 3;

  bla::Matrix<double> C = A;

  std::cout << C << std::endl << C.width() << C.height() << std::endl;

  B = C;
  
  std::cout << B <<std::endl;

  B = returnsmatrix(m, n);

  std::cout << B <<std::endl;

  bla::Matrix<double> E(4, 7, {1, 2, 3, 4, 5, 6, 7,
                               8, 9, 10, 11, 12, 13, 14,
                               15, 16, 17, 18, 19, 20, 21,
                               22, 23, 24, 25, 26, 27, 28});

  bla::Matrix<double, bla::ColMajor> F(7, 4, {1, 2, 3, 4, 5, 6, 7,
                               8, 9, 10, 11, 12, 13, 14,
                               15, 16, 17, 18, 19, 20, 21,
                               22, 23, 24, 25, 26, 27, 28});

  for (size_t i=0; i < E.height(); i++){
    std::cout << E.Row(i) << std::endl;
  }

  for (size_t j=0; j < E.width(); j++){
    std::cout << E.Col(j) << std::endl;
  }  

  std::cout << E << std::endl << E.width() << E.height() << std::endl << E.transposed() << std::endl << "  󱁖" << std::endl;

  std::cout << F.Rows(1, 2) << std::endl;

}

// tests for matrix expressions
void expr_tests(){

  bla::Matrix<double> A (4, 3, {0, 0, 1,
                                0, 1, 0,
                                1, 0, 0,
                                2, 1, 3});

  bla::Matrix<double> B (3, 2, {2, 0,
                                4, 1,
                                4, 2});

  bla::Vector<double> b (3);
  b(0) = 1;
  b(1) = 2;
  b(2) = 3;

std::cout << "A+B:" << std::endl;
std::cout << b << std::endl;

std::cout << "A*B:" << std::endl;
std::cout << A*B << std::endl;

std::cout << "A*b:" << std::endl;
std::cout << A*b << std::endl;
// std::cout << A+B*A << std::endl;
}

// tests for inverse calculation
void inverse_tests(){
  // https://studyflix.de/mathematik/inverse-matrix-berechnen-2432
  bla::Matrix<double> A (3, 3, {2, 0, 1,
                                4, 1, 2,
                                4, 2, 0});
  bla::Inverse(A);
  std::cout << bla::Inverse(A) << std::endl << bla::Inverse(A)*A << std::endl;

  // https://studyflix.de/mathematik/inverse-2x2-2420
  bla::Matrix<double> B (2, 2, {1, 2,
                                2, 3});
  
  // std::cout << bla::Inverse(B) << std:endl << bla::inverse(B)*B << std::endl;

}

int main()
{
try{
  

  // misc_tests();
  // expr_tests();
  inverse_tests();
  return 0;
  // TODO test Matrix(const MatrixExpr<TB> & B)
  // TODO test output stream operator
}
catch (double err){
  std::cout << "error caught: " << err << std::endl;
}

}
