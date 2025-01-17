#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <ostream>

#include "fastmult.h"
#include "matrix.h"


using namespace Neo_CLA;
using namespace std;

/* 
// is the computation correct?
void correctness_test(){

  Matrix<> A(4, 1, {1, 2, 3, 4});
  Matrix<> B(1, 12, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});

  Matrix<double> B (4, 12, {1, 2, 3, 4, 5, 6, 7, 29, 30, 31, 32, 33,
                            8, 9, 10, 11, 12, 13, 14, 34, 35, 36, 37, 38,
                            15, 16, 17, 18, 19, 20, 21, 39, 40, 41, 42, 43,
                            22, 23, 24, 25, 26, 27, 28, 44, 45, 46, 47, 48});
  
  Matrix<double> A (4, 4, {0, 0, 1, 0,
                            0, 1, 0, 0,
                            1, 0, 0, 0,
                            0, 1, 0, 1});

  Matrix<double> B (5, 13, {1, 2, 3, 4, 5, 6, 7, 29, 30, 31, 32, 33, 49,
                            8, 9, 10, 11, 12, 13, 14, 34, 35, 36, 37, 38, 50,
                            15, 16, 17, 18, 19, 20, 21, 39, 40, 41, 42, 43, 51,
                            22, 23, 24, 25, 26, 27, 28, 44, 45, 46, 47, 48, 52,
                            22, 23, 24, 25, 26, 27, 28, 44, 45, 46, 47, 48, 53});
  
  Matrix<double> A (5, 5, {0, 0, 1, 0, 1,
                            0, 1, 0, 0, 0,
                            1, 0, 0, 0, 2,
                            0, 1, 0, 1, 3,
                            1, 0, 5, 2, 1});

  Matrix<> C(5, 13);
  C = 0;

  cout << A*B << endl;

  multmatmat(C, A, B);
  cout << C << endl;
  cout << A*B - C << endl;

  Matrix<> D (50, 30);
  D = 0;
  Matrix<> E = randommatrix<>(50, 40);
  Matrix<> F = randommatrix<>(40, 30);
  multmatmat(D, E, F);
  std::cout << E*F - D << std::endl;
} */

void performance_test(){

  size_t n = 1000;

  Matrix<> A = randommatrix<>(n, n);
  Matrix<> B = randommatrix<>(n, n);
  Matrix<> C (n, n);
  C = 0;

  auto start = std::chrono::high_resolution_clock::now();

  multparallel_timed(C, A, B); // the computation
  
  auto end = std::chrono::high_resolution_clock::now();
  double time = std::chrono::duration<double>(end-start).count();

  // stats output
  double GFlops = (n*n*(n - 1))/(time*1e9);
  std::cout << "n = " << n << ", time = " << time << " s, GFlops = " << GFlops << std::endl;

  if (GFlops <= 4) std::cout << "Not enough performance? Do not forget to compile with cmake \"-DCMAKE_BUILD_TYPE=Release ..\"!" << std::endl;

  // is the result correct? if yes, all values of C - A*B lie around 0
  Matrix D = C - A*B;
  const auto [min, max] = std::minmax_element(D.Data(), D.Data() + D.height()*D.width());
  std::cout << "max error: " << std::max(std::abs(*min), std::abs(*max)) << std::endl;
}


int main (){

  // correctness_test();
  performance_test();

  return 0;
}