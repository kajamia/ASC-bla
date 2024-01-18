#ifndef FILE_FASTMULT_H
#define FILE_FASTMULT_H

#include <algorithm>
#include <iostream>
#include <mutex>

#include "matrix.h"
#include "simd.h"
#include "simd_avx.h"
#include "taskmanager.cc"
#include "timer.cc"


namespace Neo_CLA{

using namespace Neo_HPC;

// The multiplication kernel is the centerpiece of matrix multiplication implemented here,
// it provides the fastest 4x12 scalar product.
// C += A*B
template <ORDERING ORD>
void multkernel(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  // A needs to have height 4
  // the width of A needs to be the height of B
  // B needs to have width 12
  // C needs to have height 4 and width 12

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
    }
  }
}


// matrix product on a matrix block that is (potentially) smaller than 4x12, used to clean up leftover
// almost the same as multkernel above, with added masks
template <ORDERING ORD>
void smallblock(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  // almost the same as in multkernel, but with masks
  for (size_t i=0; i < B.height(); i++)
  {
    SIMD<int64_t, 4> sequ0 = IndexSequence<int64_t, 4, 0>();
    SIMD<int64_t, 4> sequ1 = IndexSequence<int64_t, 4, 4>();
    SIMD<int64_t, 4> sequ2 = IndexSequence<int64_t, 4, 8>();

    SIMD<mask64, 4> mask0 = (B.width() > sequ0);
    SIMD<mask64, 4> mask1 = (B.width() > sequ1);
    SIMD<mask64, 4> mask2 = (B.width() > sequ2);

    // loads using masks
    SIMD<double, 4> b0(&B(i, 0), mask0);
    SIMD<double, 4> b1(&B(i, 4), mask1);
    SIMD<double, 4> b2(&B(i, 8), mask2);

    for (size_t k=0; k < A.height(); k++)
    {
      // now, we have picked out an element of A and a line of B
      // loading and storing is done with masking off the components of C that do not actually exist
      FMA(SIMD<double, 4>(A(k, i)), b0, SIMD<double, 4>(&C(k, 0), mask0)).Store(&C(k, 0), mask0);
      FMA(SIMD<double, 4>(A(k, i)), b1, SIMD<double, 4>(&C(k, 4), mask1)).Store(&C(k, 4), mask1);
      FMA(SIMD<double, 4>(A(k, i)), b2, SIMD<double, 4>(&C(k, 8), mask2)).Store(&C(k, 8), mask2);
    }
  }
}


// This function computes a matrix product by passing individual blocks to multkernel/smallblock.
template <ORDERING ORD>
void multmatmat(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  // divide the large multiplication into blocks
  // dimensions: C: mxn ; A: mxk ; B: kxn
  size_t m = C.height();
  size_t n = C.width();
  size_t k = A.width();
  
  // how many full 4x12 blocks can we cram into C?
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

  // rightmost columns
  for (size_t i=0; i < fullblocks_height; i++)
  {
    smallblock(C.Rows(i*4, 4).Cols(fullblocks_width*12, n%12), A.Rows(i*4, 4), B.Cols(fullblocks_width*12, n%12));
  }

  // rows all the way at the bottom
  for (size_t j=0; j < fullblocks_width; j++)
  {
    smallblock(C.Rows(fullblocks_height*4, m%4).Cols(j*12, 12), A.Rows(fullblocks_height*4, m%4), B.Cols(j*12, 12));
  }

  // bottom-right corner
  if ((m % 4) != 0 && (n % 4) != 0)
  {
    smallblock(C.Rows(fullblocks_height*4, m%4).Cols(fullblocks_width*12, 12), A.Rows(fullblocks_height*4, m%4), B.Cols(fullblocks_width*12, n%12));
  }
}

// CACHING ---------------------------------------------------------------------

// computes the product for a block that can be filled with 4x12 matrices
// helper function for multcachy
template <ORDERING ORD, typename H = std::integral_constant<size_t, 4>, typename W = std::integral_constant<size_t, 12> >
void blockmultcachy(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  W w;
  H h;

  for (size_t j=0; j+w <= C.width(); j += w)
  {
    for (size_t i=0; i+h <= C.height(); i += h)
    {
      multkernel(C.Rows(i, h).Cols(j, w), A.Rows(i, h), B.Cols(j, w));
    }
  }

  // cleanup:

  for (size_t i=0; i+h <= C.height(); i += h) // rightmost columns
    smallblock(C.Rows(i, h).Cols(C.width() - C.width()%w, C.width()%w), A.Rows(i, h), B.Cols(C.width() - C.width()%w, C.width()%w));

  for (size_t j=0; j+w <= C.width(); j += w) // bottom rows
    smallblock(C.Rows(C.height() - C.height()%h, C.height()%h).Cols(j, w), A.Rows(C.height() - C.height()%h, C.height()%h), B.Cols(j, w));
  
  if ((C.height() % h) != 0 && (C.width() % w) != 0) // lower-right corner
    smallblock(C.Rows(C.height() - C.height()%h, C.height()%h).Cols(C.width() - C.width()%w, C.width()%w), A.Rows(C.height() - C.height()%h, C.height()%h), B.Cols(C.width() - C.width()%w, C.width()%w));
}

// same as multmatmat, but with caching
// BH and BW are the height and width of the matrix block that is supposed to stay in memory,
// they can be adjusted to fit the cache size (96 makes blocks of A fit into L2-Cache)
template <ORDERING ORD, typename BH = std::integral_constant<size_t, 96>, typename BW = std::integral_constant<size_t, 96> >
void multcachy(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  BH bh;
  BW bw;

  alignas (64) double memA[bh*bw]; // memory for Ablock

  for (size_t i1 = 0; i1 < A.height(); i1 += bh) {
    for (size_t j1 = 0; j1 < A.width(); j1 += bw) {
      // end indices of block to be computed:
      size_t i2 = std::min(A.height(), i1+bh);
      size_t j2 = std::min(A.width(), j1+bw);

      // copies a block to stay in cache
      MatrixView Ablock(i2-i1, j2-j1, bw, memA);
      Ablock = A.Rows(i1,i2-i1).Cols(j1,j2-j1);
      
      // computes the product for a block of maximum bhxbw
      blockmultcachy (C.Rows(i1, i2-i1), Ablock, B.Rows(j1, j2-j1));
    }
  }
}


// PARALLELIZATION -------------------------------------------------------------

// the most powerful function
// the same as multcachy, but with threads instead of loops
template <typename BH = std::integral_constant<size_t, 96>, typename BW = std::integral_constant<size_t, 96>, ORDERING ORD>
void multparallel(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  BH bh;
  BW bw;

  // as many workers as threads supported by your machine, minus one
  StartWorkers(std::thread::hardware_concurrency() - 1);

  RunParallel((A.height()/bh) + 1, [&](int i0, int s){    
    std::mutex linemutex; // prevents race condition in kernel and memA
    alignas (64) double memA[bh*bw]; // if larger lines are locked, not that much allocation is necessary

    RunParallel((A.width()/bw) + 1, [&](int j0, int s2){
      
      // indices for the block matrices
      size_t i1 = i0*bh;
      size_t j1 = j0*bw;
      size_t i2 = std::min(A.height(), i1+bh);
      size_t j2 = std::min(A.width(), j1+bw);

      MatrixView Ablock(i2-i1, j2-j1, bw, memA);

      std::lock_guard<std::mutex> lock(linemutex); // lock

      Ablock = A.Rows(i1,i2-i1).Cols(j1,j2-j1);
      
      blockmultcachy (C.Rows(i1, i2-i1), Ablock, B.Rows(j1, j2-j1));
    });
  });

  StopWorkers();
}

// a variant of multparallel that creates performance statistics
template <typename BH = std::integral_constant<size_t, 96>, typename BW = std::integral_constant<size_t, 96>, ORDERING ORD>
void multparallel_timed(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)
{
  BH bh;
  BW bw;

  timeline = std::make_unique<TimeLine>("fastmult.trace");
  static Timer t("fastmult", {1, 0, 0});

  // as many workers as threads supported by your machine, minus one
  StartWorkers(std::thread::hardware_concurrency() - 1);

  RunParallel((A.height()/bh) + 1, [&](int i0, int s){    
    std::mutex linemutex; // prevents race condition in kernel and memA
    alignas (64) double memA[bh*bw]; // if larger lines are locked, not that much allocation is necessary

    RunParallel((A.width()/bw) + 1, [&](int j0, int s2){
      RegionTimer reg(t);
      
      // indices for the block matrices
      size_t i1 = i0*bh;
      size_t j1 = j0*bw;
      size_t i2 = std::min(A.height(), i1+bh);
      size_t j2 = std::min(A.width(), j1+bw);

      MatrixView Ablock(i2-i1, j2-j1, bw, memA);

      std::lock_guard<std::mutex> lock(linemutex); // lock

      Ablock = A.Rows(i1,i2-i1).Cols(j1,j2-j1);
      
      blockmultcachy (C.Rows(i1, i2-i1), Ablock, B.Rows(j1, j2-j1));
    });
  });

  StopWorkers();
}


} // namespace
#endif
