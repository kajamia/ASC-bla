#include <iostream>

#include <vector.h>
#include <lapack_interface.h>
#include <matrix.h>

using namespace ASC_bla;
using namespace std;

void testmatmul() {
  bla::Matrix<double> A (4, 3, {0, 0, 1,
                                0, 1, 0,
                                1, 0, 0,
                                2, 1, 3});

  bla::Matrix<double> B (3, 2, {2, 0,
                                4, 1,
                                4, 2});

  Matrix C (4, 2);
  std::cout << MultMatMatLapack (A,B,C) << std::endl;

}


int main()
{
  Vector<double> x(5);
  Vector<double> y(5);

  for (int i = 0; i < x.Size(); i++)
    {
      x(i) = i;
      y(i) = 2;
    }

  cout << "x = " << x << endl;
  cout << "y = " << y << endl;
  
  AddVectorLapack (2, x, y);  
  cout << "y+2*x = " << y << endl;

  void testmatmul();
}

  
