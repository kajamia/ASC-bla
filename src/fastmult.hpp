#include <iostream>
#include <algorithm>

#include "matrix.h"
#include "simd.h"
#include "simd_avx.h"


namespace Neo_CLA{

using namespace Neo_HPC;

// The multiplication kernel is the centerpiece of matrix multiplication,
// it provides the fastest multiple scalar product.
// C += A*B
template <ORDERING ORD>
void multkernel(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  // A needs to have height 4
  // the width of A needs to be the height of B
  // B needs to have width 12
  // C needs to have height 4 and width 12

  // std::cout << B.Col(0) << std::endl;
  // std::cout << B.height() << ", " << B.width() << std::endl;

  for (size_t i=0; i < B.height(); i++)
  {
    // rows of B are stored as 3 SIMD-vectors, A is accessed without SIMD
    SIMD<double, 4> b0(&B(i, 0));
    SIMD<double, 4> b1(&B(i, 4));
    SIMD<double, 4> b2(&B(i, 8));

    for (size_t k=0; k < 4; k++)
    {
      // now, we have picked out an element of A and a line of B
      FMA(SIMD<double, 4>(A(k, i)), b0, SIMD<double, 4>(&C(k, 0))).Store(&C(k, 0));
      FMA(SIMD<double, 4>(A(k, i)), b1, SIMD<double, 4>(&C(k, 4))).Store(&C(k, 4));
      FMA(SIMD<double, 4>(A(k, i)), b2, SIMD<double, 4>(&C(k, 8))).Store(&C(k, 8));
      // std::cout << C(0, 0) << std::endl;
      // if (k == 0) std::cout << A(k, i) << std::endl;
    }
  }
}


// matrix product on a small block, used to clean up leftover
// almost the same as multkernel above, with added masks
template <ORDERING ORD>
void smallblock(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  // almost the same as in multkernel, but with the option of masks
  for (size_t i=0; i < B.height(); i++)
  {
    SIMD<int64_t, 4> sequ0 = IndexSequence<int64_t, 4, 0>();
    SIMD<int64_t, 4> sequ1 = IndexSequence<int64_t, 4, 4>();
    SIMD<int64_t, 4> sequ2 = IndexSequence<int64_t, 4, 8>();

    SIMD<mask64, 4> mask0 = (B.width() > sequ0);
    SIMD<mask64, 4> mask1 = (B.width() > sequ1);
    SIMD<mask64, 4> mask2 = (B.width() > sequ2);

    SIMD<double, 4> b0(&B(i, 0));
    SIMD<double, 4> b1(&B(i, 4));
    SIMD<double, 4> b2(&B(i, 8));

    for (size_t k=0; k < A.height(); k++)
    {
      // now, we have picked out an element of A and a line of B
      FMA(SIMD<double, 4>(A(k, i)), b0, SIMD<double, 4>(&C(k, 0), mask0)).Store(&C(k, 0), mask0);
      FMA(SIMD<double, 4>(A(k, i)), b1, SIMD<double, 4>(&C(k, 4), mask1)).Store(&C(k, 4), mask1);
      FMA(SIMD<double, 4>(A(k, i)), b2, SIMD<double, 4>(&C(k, 8), mask2)).Store(&C(k, 8), mask2);
    }
  }
}


// This function computes a matrix product by passing individual blocks to multkernel.
template <ORDERING ORD>
void multmatmat(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  // divide the large multiplication into blocks
  // dimensions: C: mxn ; A: mxk ; B: kxn
  size_t m = C.height();
  size_t n = C.width();
  size_t k = A.width();
  
  // how many full blocks (4x12 within C) can we cram in?
  size_t fullblocks_height = (m - (m % 4)) / 4;
  size_t fullblocks_width = (n - (n % 12)) / 12;

  for (size_t i=0; i < fullblocks_height; i++)
  {
    for (size_t j=0; j < fullblocks_width; j++)
    {
      // now, we compute
      multkernel(C.Rows(i*4, 4).Cols(j*12, 12), A.Rows(i*4, 4), B.Cols(j*12, 12));
    }
  }

  // cleanup:

  for (size_t i=0; i < fullblocks_height; i++)
  {
    smallblock(C.Rows(i*4, 4).Cols(fullblocks_width*12, n%12), A.Rows(i*4, 4), B.Cols(fullblocks_width*12, n%12));
  }

  for (size_t j=0; j < fullblocks_width; j++)
  {
    smallblock(C.Rows(fullblocks_height*4, m%4).Cols(j*12, 12), A.Rows(fullblocks_height*4, m%4), B.Cols(j*12, 12));
  }

  if ((m % 4) != 0 && (n % 4) != 0)
  {
    smallblock(C.Rows(fullblocks_height*4, m%4).Cols(fullblocks_width*12, 12), A.Rows(fullblocks_height*4, m%4), B.Cols(fullblocks_width*12, n%12));
  }
}

/* 
// same as multmatmat, but with caching
// BH and BW are the height and width of the matrix block that is supposed to stay in memory,
// they can be adjusted to fit the cache size (96 makes blocks of A fit into L2-Cache)
template <ORDERING ORD, typename BH = std::integral_constant<size_t, 96>, typename BW = std::integral_constant<size_t, 96> >
void multcachy(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  // BH and BW are the height and width of the matrix block that is supposed to stay in memory,
  // they can be adjusted to fit the cache size (96 makes blocks of A fit into L2-Cache)
  constexpr size_t BH=96;
  constexpr size_t BW=96;
  BH bh;
  BW bw;

  size_t m = A.height();
  size_t n = A.width();

  // how many full blocks (96x96 within A) can we cram in?
  size_t fullblocks_height = (m - (m % bh)) / bh;
  size_t fullblocks_width = (n - (n % bw)) / bw;

  for (size_t i=0; i < fullblocks_height; i++)
  {
    for (size_t j=0; j < fullblocks_width; j++)
    {
      multmatmat(C.Rows(i*bh, bh), A.Rows(i*bh, bh).Cols(j*bw, bw), B.Rows(j*bw, bw));
    }
  }

  // cleanup

  for (size_t i=0; i < fullblocks_height; i++)
  {
    multmatmat(C.Rows(i*bh, bh), A.Rows(i*bh, bh).Cols((fullblocks_width + 1)*bw, n%bw), B.Rows((fullblocks_width + 1)*bw, n%bw));
  }

  for (size_t j=0; j < fullblocks_width; j++)
  {
    multmatmat(C.Rows((fullblocks_height + 1)*bh, n%bh), A.Rows((fullblocks_height + 1)*bh, n%bh).Cols(j*bw, bw), B.Rows(j*bw, bw));
  }

  if ((m % bh) != 0 && (m % bw) != 0)
  {
    multmatmat(C.Rows((fullblocks_height + 1)*bh, n%bh), A.Rows((fullblocks_height + 1)*bh, n%bh).Cols((fullblocks_width + 1)*bw, n%bw), B.Rows((fullblocks_width + 1)*bw, n%bw));
  }
}
 */

template <ORDERING ORD, typename H = std::integral_constant<size_t, 4>, typename W = std::integral_constant<size_t, 12> >
void blockmultcachy(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  W w;
  H h;

  for (size_t j=0; j+w <= C.width(); j += w)
  {
    for (size_t i=0; i+h <= C.height(); i += h)
    {
      multkernel(C.Rows(i, h).Cols(j, w), A.Rows(i, h), B.Cols(j, w)); // A.width(), &A(i,0), A.Dist(), &B(0,j), B.Dist(), &C(i,j), C.Dist()
      // std::cout << i << ", " << j << std::endl;

      /* if (i == 1*h && j == 0){
        return;
      } */
    }
  }

  // cleanup

  for (size_t i=0; i+h <= C.height(); i += h)
    smallblock(C.Rows(i, h).Cols(C.width() - C.width()%w, C.width()%w), A.Rows(i, h), B.Cols(C.width() - C.width()%w, C.width()%w));

  for (size_t j=0; j+w <= C.width(); j += w)
    smallblock(C.Rows(C.height() - C.height()%h, C.height()%h).Cols(j, w), A.Rows(C.height() - C.height()%h, C.height()%h), B.Cols(j, w));
  
  if ((C.height() % h) != 0 && (C.width() % w) != 0)
    smallblock(C.Rows(C.height() - C.height()%h, C.height()%h).Cols(C.width() - C.width()%w, C.width()%w), A.Rows(C.height() - C.height()%h, C.height()%h), B.Cols(C.width() - C.width()%w, C.width()%w));
}

template <ORDERING ORD, typename BH = std::integral_constant<size_t, 96>, typename BW = std::integral_constant<size_t, 96> >
void multcachy(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  BH bh;
  BW bw;

  alignas (64) double memBA[bh*bw];

  for (size_t i1 = 0; i1 < A.height(); i1 += bh) {
    for (size_t j1 = 0; j1 < A.width(); j1 += bw) {
      size_t i2 = std::min(A.height(), i1+bh);
      size_t j2 = std::min(A.width(), j1+bw);

      MatrixView Ablock(i2-i1, j2-j1, bw, memBA);
      Ablock = A.Rows(i1,i2-i1).Cols(j1,j2-j1);
      // std::cout << B.Col(0) << std::endl;
      // std::cout << C << std::endl << B.Rows(j1,j2-j1) << Ablock << std::endl;
      blockmultcachy (C.Rows(i1, i2-i1), Ablock, B.Rows(j1, j2-j1));
    }
  }
}

/* template <ORDERING ORD, typename BH = std::integral_constant<size_t, 96>, typename BW = std::integral_constant<size_t, 96> >
void multparalleltimed(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  
} */

} // namespace
