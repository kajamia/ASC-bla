#include <iostream>

#include "matrix.h"
#include "simd.h"
#include "simd_avx.h"


namespace Neo_CLA{



template <typename T, ORDERING ORD>
void multkernel(MatrixView<T, RowMajor> C, MatrixView<T, RowMajor> A, MatrixView<T, ORD> B){
  // C needs to have size ??? -> error handling!

}

} // namespace
