#ifndef FILE_LAPACK_INTERFACE_H
#define FILE_LAPACK_INTERFACE_H

#include <iostream>
#include <string>

#include "vector.h"
#include "matrix.h"



#include <complex>
#include <vector>

typedef int integer;
typedef integer logical;
typedef float real;
typedef double doublereal;
typedef std::complex<float> singlecomplex;
typedef std::complex<double> doublecomplex;

// Windows SDK defines VOID in the file WinNT.h
#ifndef VOID
typedef void VOID;
#endif
typedef int ftnlen;
typedef int L_fp;  // ?


extern "C" {
#include "clapack.h"
}




namespace ASC_bla
{
  
  // BLAS-1 functions:

  /*
    int daxpy_(integer *n, doublereal *da, doublereal *dx, 
    integer *incx, doublereal *dy, integer *incy);
  */
  // y += alpha x
  template <typename SX, typename SY>
  void AddVectorLapack (double alpha, VectorView<double,SX> x, VectorView<double,SY> y)
  {
    integer n = x.Size();
    integer incx = x.Dist();
    integer incy = y.Dist();
    int err = 
      daxpy_ (&n, &alpha, &x(0),  &incx, &y(0), &incy);
   
   // if (err != 0)
   //   throw std::runtime_error(std::string("daxpy returned errcode "+std::to_string(err)));      
  }
  
  // InnerProductLapack, NormLapack


  // BLAS-2 functions:
  // MultMatVecLapack



  // BLAS-3 functions:
  // MultMatMatLapack
  
  // int dgemm_ (char *transa, char *transb, integer *m, integer * n,
  // integer *k, doublereal *alpha, doublereal *a, integer *lda, 
  // doublereal *b, integer *ldb, doublereal *beta, doublereal *c__, 
  // integer *ldc);

   
  // c = a*b
  template <ORDERING OA, ORDERING OB>
  void MultMatMatLapack (MatrixView<double, OA> a,
                         MatrixView<double, OB> b,
                         MatrixView<double, ColMajor> c)
  {
    char transa_ = (OA == ColMajor) ? 'N' : 'T';
    char transb_ = (OB == ColMajor) ? 'N' : 'T'; 
  
    integer n = c.height();
    integer m = c.width();
    integer k = a.width();
  
    double alpha = 1.0;
    double beta = 0;
    integer lda = std::max(a.Dist(), 1ul);
    integer ldb = std::max(b.Dist(), 1ul);
    integer ldc = std::max(c.Dist(), 1ul);

    int err =
      dgemm_ (&transa_, &transb_, &n, &m, &k, &alpha, 
              a.Data(), &lda, b.Data(), &ldb, &beta, c.Data(), &ldc);

    if (err != 0)
      throw std::runtime_error(std::string("MultMatMat got error "+std::to_string(err)));
  }

  // multiplication for row-major c         
  template <ORDERING OA, ORDERING OB>
  void MultMatMatLapack (MatrixView<double, OA> a,
                        MatrixView<double, OB> b,
                        MatrixView<double, RowMajor> c)
  {
    MultMatMatLapack (b.transposed(), a.transposed(), c.transposed());
  }

  class T_Lapack { };
  static constexpr T_Lapack Lapack;

  // | operator overload



  //a.Height() - a.SizeRows()
  //a.Width() - a.SizeCols()

  template <ORDERING ORD>
  class LapackLU {
    Matrix <double, ORD> a;
    std::vector<integer> ipiv;

  public:
    LapackLU (Matrix<double,ORD> _a, bool apply = true)
    : a(std::move(_a)), ipiv(a.SizeRows()) {
    //constructor, if flag apply true then decompose, if False decompose only when needed
      if (apply){}
        integer m = a.SizeRows();
        if (m == 0) return;
        integer n = a.SizeCols();
        integer lda = a.Dist();
        integer info;
    
      //int dgetrf_(integer *m, integer *n, doublereal *a, 
      //             integer * lda, integer *ipiv, integer *info);

      dgetrf_(&n, &m, &a(0,0), &lda, &ipiv[0], &info);
    }


    // b overwritten with A^{-1} b
    void Solve (VectorView<double> b) const {
      char transa =  (ORD == ColMajor) ? 'N' : 'T';
      integer n = a.SizeRows();
      integer nrhs = 1;
      integer lda = a.Dist();
      integer ldb = b.Size();
      integer info;

      // int dgetrs_(char *trans, integer *n, integer *nrhs, 
      //             doublereal *a, integer *lda, integer *ipiv,
      //             doublereal *b, integer *ldb, integer *info);

      dgetrs_(&transa, &n, &nrhs, a.Data(), &lda, (integer*)ipiv.data(), b.Data(), &ldb, &info);
    }
  

    Matrix<double,ORD> Inverse() && {
    double hwork;
    integer lwork = -1;
    integer n = a.SizeRows();      
    integer lda = a.Dist();
    integer info;
    /*
    int dgetri_(integer *n, doublereal *a, integer *lda, 
                integer *ipiv, doublereal *work, integer *lwork, 
                integer *info);
    */       
    dgetri_(&n, &a(0,0), &lda, ipiv.data(), &hwork, &lwork, &info);
    lwork = integer(hwork);
    std::vector<double> work(lwork);
    dgetri_(&n, &a(0,0), &lda, ipiv.data(), &work[0], &lwork, &info);
    return std::move(a);  
  
    }

    /*
    Matrix<double,ORD> LFactor() const { ... }
    Matrix<double,ORD> UFactor() const { ... }
    Matrix<double,ORD> PFactor() const { ... }
    */

    Matrix<double, ORD> LFactor() const {
      Matrix<double, ORD> L(a.SizeRows(), a.SizeCols());
      for (size_t i = 0; i < a.SizeRows(); i++) {
        for (size_t j = 0; j < a.SizeCols(); j++) {
          if (i > j)
            L(i, j) = a(i, j);
          else if (i == j)
            L(i, j) = 1;
          else
            L(i, j) = 0;
        }
      }
      return L;
    };

    Matrix<double, ORD> UFactor() const {
      Matrix<double, ORD> U(a.SizeRows(), a.SizeCols());
      for (size_t i = 0; i < a.SizeRows(); i++) {
        for (size_t j = 0; j < a.SizeCols(); j++) {
          if (i <= j)
            U(i, j) = a(i, j);
          else
            U(i, j) = 0;
        }
      }
      return U;
    };

    Matrix<double, ORD> PFactor() const {
      Matrix<double, ORD> P(a.SizeRows(), a.SizeCols());
      for (size_t i = 0; i < a.SizeRows(); i++) {
        for (size_t j = 0; j < a.SizeCols(); j++) {
          if (i == ipiv[j])
            P(i, j) = 1;
          else
            P(i, j) = 0;
        }
      }
      return P;

    };
  };

  /*
  //Other examples for useful matrix decompositions are QR-factorization
  //From LU

  template <ORDERING ORD>
  class LapackQR {
    Matrix <double, ORD> a;
    std::vector<double> ipiv;


  */
};
#endif