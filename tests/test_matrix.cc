#include <iostream>

#include <matrix.h>
#include <matrix_expression.h>

namespace bla = ASC_bla;

// for testing move semantics
bla::Matrix<double, bla::RowMajor> returnsmatrix(size_t m, size_t n){
  bla::Matrix<double, bla::RowMajor> D(m, n);
  D(3, 3) = 5.6;
  return D;
}

int main()
{
  size_t m = 6;
  size_t n = 5;
  bla::Matrix<double, bla::RowMajor> A(m, n);
  bla::Matrix<double, bla::ColMajor> B(m, n);

  A(2, 2) = 3;

  bla::Matrix<double, bla::RowMajor> C = A;

  std::cout << C << std::endl << C.width() << C.height() << std::endl;

  B = C;
  
  std::cout << B <<std::endl;

  B = returnsmatrix(m, n);

  std::cout << B <<std::endl;

  /*
  for (size_t i = 0; i < x.Size(); i++)
    {
      x(i) = i;
      y(i) = 10;
    }

  bla::Matrix<double> z = x+y;

  std::cout << "x+y = " << z << std::endl;
  */
}
