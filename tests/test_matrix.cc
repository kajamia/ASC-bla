#include <iostream>

#include <matrix.h>

namespace bla = ASC_bla;

int main()
{
  size_t m = 6;
  size_t n = 5;
  bla::Matrix<double, bla::RowMajor> A(m, n);
  bla::Matrix<double, bla::ColMajor> B(m, n);

  bla::Matrix<double, bla::RowMajor> C = A;
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
