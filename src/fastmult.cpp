#include <iostream>

#include "matrix.h"
#include "../external_dependencies/HPC/src/simd.h"
#include "../external_dependencies/HPC/src/simd_avx.h"


namespace ASC_bla{

// the fastest loop from Neo-CLA-HPC/demos/simd_timings.cc
template <size_t SW = std::integral_constant<size_t, 16> >
auto InnerProduct2 (size_t n, double * px0, double * px1, double * py, size_t dy)
{
  SIMD<double,SW> sum0{0.0};
  SIMD<double,SW> sum1{0.0};
  for (size_t i = 0; i < n; i++)
    {
      // sum += px[i] * SIMD<double,SW>(py+i*dy);
      SIMD<double,SW> yi(py+i*dy);
      sum0 = FMA(SIMD<double,SW>(px0[i]), yi, sum0);
      sum1 = FMA(SIMD<double,SW>(px1[i]), yi, sum1);
    }
  return tuple(sum0, sum1);
}


template <typename T, ORDERING ORD>
void multkernel(MatrixView<T, RowMajor> C, MatrixView<T, RowMajor> A, MatrixView<T, ORD> B){
  // C needs to have size ???

  SIMD<double,SW> sum{0.0};
  auto [sum0,sum1] = InnerProduct2<SW> (n, px0, px1, py, SW);
  sum += sum0 + sum1;
}

} // namespace
