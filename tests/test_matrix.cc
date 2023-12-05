#include <iostream>
#include <exception>

#include "matrix.h"
#include "matrix_expression.h"
#include "vector.h"
#include "simd.h"

namespace cla = Neo_CLA;

// for testing move semantics
cla::Matrix<double, cla::RowMajor> returnsmatrix(size_t m, size_t n){
  cla::Matrix<double, cla::RowMajor> D(m, n);
  D(3, 3) = 5.6;
  return D;
}

// miscellaneous tests (storage, access, output, ...)
void misc_tests(){

  size_t m = 6;
  size_t n = 5;
  cla::Matrix<double, cla::RowMajor> A(m, n);
  cla::Matrix<double, cla::ColMajor> B(m, n);
  
  A = 3.14;
  A(2, 2) = 3;

  cla::Matrix<double> C = A;

  // std::cout << C << std::endl << C.width() << C.height() << std::endl;

  B = C;
  
  // std::cout << B <<std::endl;

  B = returnsmatrix(m, n);

  // std::cout << B <<std::endl;

  cla::Matrix<double> E(4, 7, {1, 2, 3, 4, 5, 6, 7,
                               8, 9, 10, 11, 12, 13, 14,
                               15, 16, 17, 18, 19, 20, 21,
                               22, 23, 24, 25, 26, 27, 28});

  cla::Matrix<double, cla::ColMajor> F(7, 4, {1, 2, 3, 4, 5, 6, 7,
                               8, 9, 10, 11, 12, 13, 14,
                               15, 16, 17, 18, 19, 20, 21,
                               22, 23, 24, 25, 26, 27, 28});
  
  std::cout << "Diag of F=" << F.Diag() << std::endl;

/*   for (size_t i=0; i < E.height(); i++){
    std::cout << E.Row(i) << std::endl;
  }

  for (size_t j=0; j < E.width(); j++){
    std::cout << E.Col(j) << std::endl;
  }  

  std::cout << E << std::endl << E.width() << E.height() << std::endl << E.transposed() << std::endl << "  󱁖" << std::endl;

  std::cout << F.Rows(1, 2) << std::endl */;
  // std::cout << F << F.transposed();

  /* cla::Matrix<double, cla::RowMajor> G (4, 7);
  G = 42;

  cla::MatrixView H(E.Cols(1, 2));
  cla::MatrixView I(G.Cols(1, 2));

  std::cout << H << std::endl << I << std::endl;

  H = I;

  std::cout << H.Data() << std::endl << I.Data() << std::endl;

  std::cout << E << std::endl; */

  A+E;

}

// tests for matrix expressions
void expr_tests(){

  cla::Matrix<double> A (4, 3, {0, 0, 1,
                                0, 1, 0,
                                1, 0, 0,
                                2, 1, 3});

  cla::Matrix<double> B (3, 2, {2, 0,
                                4, 1,
                                4, 2});

  cla::Vector<double> b (3);
  b(0) = 1;
  b(1) = 2;
  b(2) = 3;

  std::cout << "b: " << std::endl;
  std::cout << b << std::endl;

  std::cout << "A*B:" << std::endl;
  std::cout << A*B << std::endl;

  std::cout << "A*b:" << std::endl;
  std::cout << A*b << std::endl;

  std::cout << "33*A:" << std::endl;
  std::cout << 33*A << std::endl;

  std::cout << "33*A + 2*A:" << std::endl;
  std::cout << 33*A + 2*A << std::endl;
  
}

// tests for inverse calculation
void inverse_tests(){
  // https://studyflix.de/mathematik/inverse-matrix-berechnen-2432
  cla::Matrix<double> A (3, 3, {2, 0, 1,
                                4, 1, 2,
                                4, 2, 0});
  std::cout << cla::Inverse(A) << std::endl << cla::Inverse(A)*A << std::endl;

  // https://studyflix.de/mathematik/inverse-2x2-2420
  cla::Matrix<double> B (2, 2, {1, 2,
                                2, 3});
  
  std::cout << cla::Inverse(B) << std::endl << cla::Inverse(B)*B << std::endl;

}

int main()
{
try{
  

  misc_tests();
  // expr_tests();
  // inverse_tests();
  return 0;
  // TODO test Matrix(const MatrixExpr<TB> & B)
  // TODO test output stream operator
}
catch (const std::exception & err){
  std::cerr << "\033[31;1;4;5mERROR CAUGHT: " << err.what() << "\033[0m" << std::endl;
}

}
