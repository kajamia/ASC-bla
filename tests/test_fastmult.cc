#include <iostream>
#include <chrono>
#include <ostream>

#include "matrix.h"
#include "fastmult.cpp"


using namespace Neo_CLA;

void precision_test(){
  Matrix<double> A (4, 7, {1, 2, 3, 4, 5, 6, 7,
                            8, 9, 10, 11, 12, 13, 14,
                            15, 16, 17, 18, 19, 20, 21,
                            22, 23, 24, 25, 26, 27, 28});
  
  Matrix<double> B (4, 4, {0, 0, 1, 0,
                            0, 1, 0, 0,
                            1, 0, 0, 0,
                            0, 1, 0, 1});

  std::cout << B*A << std::endl;
  // std::cout << fastmult(B, A) << std::endl;  
}

void performance_test(){

  size_t n = 100;

  Matrix A(n, n);
  Matrix B(n, n);

  std::ostream my_stream(nullptr);

  auto start = std::chrono::high_resolution_clock::now();

  // if we don't do anything with the matrix product,
  // the compiler ignores the command to compute the product
  //my_stream << fastmult(A, B);
  
  auto end = std::chrono::high_resolution_clock::now();
  double time = std::chrono::duration<double>(end-start).count();

  int GFlops = (n*n*(n*(n-1)))/time*1e-9;

  std::cout << "n = " << n << ", time = " << time << " s, GFlops = " << GFlops << std::endl;

}


int main (){

  precision_test();
  performance_test();

  return 0;
}