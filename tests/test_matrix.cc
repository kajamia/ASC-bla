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

int main()
{
try{
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

  /* for (size_t i=0; i < E.height(); i++){
    std::cout << E.Row(i) << std::endl;
  }

  for (size_t j=0; j < E.width(); j++){
    std::cout << E.Col(j) << std::endl;
  } */  

  // std::cout << E << std::endl << E.width() << E.height() << std::endl << E.transposed() << std::endl << "  󱁖" << std::endl;

  std::cout << F.Rows(1, 2) << std::endl;


  // TODO test Matrix(const MatrixExpr<TB> & B)
  // TODO test output stream operator
}
catch (double err){
  std::cout << "error caught: " << err << std::endl;
}

}
