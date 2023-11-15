#include <iostream>
#include <chrono>

#include <vector.h>
#include <lapack_interface.h>
#include <matrix.h>

using namespace ASC_bla;
using namespace std;

void testmatmul() {
  Matrix<double> A (4, 3, {0, 0, 1,
                            0, 1, 0,
                            1, 0, 0,
                            2, 1, 3});

  Matrix<double> B (3, 2, {2, 0,
                            4, 1,
                            4, 2});

  Matrix C (4, 2);
  MultMatMatLapack (A,B,C);
  std::cout << C << std::endl;

}

void multexpr(size_t n){
  Matrix<double> A (n, n);

  Matrix<double> B (n, n);

  Matrix C = A*B;

}
void multlapack(size_t n){
 Matrix<double> A (n, n);

  Matrix<double> B (n, n);

  Matrix C (n, n);
  MultMatMatLapack (A,B,C);
}

void timematmul(size_t n){
  
  size_t flops = n*n*n;
  size_t runs = size_t (1e9 / flops) + 1;

  auto start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < runs; i++)
    multlapack(n);
  auto end = std::chrono::high_resolution_clock::now();
  double time = std::chrono::duration<double>(end-start).count();
          
  cout << "lapack: " << "n = " << n << ", time = " << time << " s, GFlops = " 
      << (flops*runs)/time*1e-9 << endl;

  start = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < runs; i++)
    multexpr(n);
  end = std::chrono::high_resolution_clock::now();
  time = std::chrono::duration<double>(end-start).count();
          
  cout << "expression: " << "n = " << n << ", time = " << time << " s, GFlops = " 
      << (flops*runs)/time*1e-9 << endl;
}

int generictests()
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

  return 0;
}

int LUtests() {
  
  // vorgerechnet in:
  // https://www.das-gelbe-rechenbuch.de/download/Lu.pdf
  Matrix<double> A (4, 4, {6, 5, 3, -10,
                           3, 7, -3, 5,
                           12, 4, 4, 4,
                           0, 12, 0, -8});
  Vector<double> b (4);
  b(0) = 1;
  b(1) = 2;
  b(2) = 3;
  b(3) = 4;
  Matrix<double> old_A(A);

  LapackLU lu(A);
  lu.Solve(b);
  std::cout << b << std::endl << old_A*b << std::endl;

  Matrix lfac = lu.LFactor();
  Matrix ufac = lu.UFactor();
  Matrix pfac = lu.PFactor();

  std::cout << "L-factor: " << std::endl << lfac << std::endl;
  std::cout << "U-factor: " << std::endl << ufac << std::endl;
  std::cout << "P-factor: " << std::endl << pfac << std::endl;

  std::cout << pfac*lfac*ufac << std::endl;


  Matrix<double> inv(lu.Inverse());
  std::cout  << inv << std::endl;

  return 0;                              
}

int main()
{
  // timematmul(100);
  LUtests();

  return 0;
}


/*
  T* cout(){return cout_;}
  std::vector<double> ipiv = LapackLU();
    cout << endl;
    fo r (int i = 0; i < ipiv.size(); i++) {
      cout << ipiv[i];
      cout << ",";
*/