#include <iostream>
#include <chrono>

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

void multexpr(){
 bla::Matrix<double> A (4, 3, {0, 0, 1,
                                0, 1, 0,
                                1, 0, 0,
                                2, 1, 3});

  bla::Matrix<double> B (3, 2, {2, 0,
                                4, 1,
                                4, 2});

  Matrix C = A*B;

}
void multlapack(){
 bla::Matrix<double> A (4, 3, {0, 0, 1,
                                0, 1, 0,
                                1, 0, 0,
                                2, 1, 3});

  bla::Matrix<double> B (3, 2, {2, 0,
                                4, 1,
                                4, 2});

  Matrix C (4, 2);
  MultMatMatLapack (A,B,C);
}

void timematmul(){
  
  size_t flops = n*n*n;
  size_t runs = size_t (1e9 / flops) + 1;

  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < runs; i++)
    multlapack()
  +auto end = std::chrono::high_resolution_clock::now();
  double time = std::chrono::duration<double>(end-start).count();
          
  cout << "lapack: " << "n = " << n << ", time = " << time << " s, GFlops = " 
      << (n*runs)/time*1e-9 << endl;

  size_t flops = n*n*n;
  size_t runs = size_t (1e9 / flops) + 1;

  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < runs; i++)
    multexpr()
  +auto end = std::chrono::high_resolution_clock::now();
  double time = std::chrono::duration<double>(end-start).count();
          
  cout << "expression: " << "n = " << n << ", time = " << time << " s, GFlops = " 
      << (n*runs)/time*1e-9 << endl;
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

  
