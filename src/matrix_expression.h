#ifndef FILE_MATRIX_EXPRESSION_H
#define FILE_MATRIX_EXPRESSION_H

#include "matrix.h"
#include "expression.h"
namespace ASC_bla
{

// T, in this case, is not the scalar but MatrixView<T, ORD> (and the latter T is the scalar type)
template <typename T>
class MatrixExpr
{ 
 public:
    auto Upcast() const { return static_cast<const T&> (*this); }
    size_t height() const { return Upcast().height(); }
    size_t width() const { return Upcast().width(); }
    auto operator() (size_t i, size_t j) const { return Upcast()(i, j); }
};

//Addition
template <typename TA, typename TB>
class SumMatrixExpr : public MatrixExpr<SumMatrixExpr<TA,TB>>
{
  TA A_;
  TB B_;
public:
  SumMatrixExpr (TA A, TB B) : A_(A), B_(B) { }

  auto operator() (size_t i, size_t j) const { return A_(i, j)+B_(i, j); }
  size_t height() const { return A_.height(); }
  size_t width() const { return A_.width(); }     
};

template <typename TA, typename TB>
auto operator+ (const MatrixExpr<TA> & A, const MatrixExpr<TB> & B)
{
  return SumMatrixExpr(A.Upcast(), B.Upcast());
}

//Multiplikation
template <typename TA, typename TB>
class ProdMatrixExpr : public MatrixExpr<ProdMatrixExpr<TA,TB>>
{
  TA A_;
  TB B_;
public:
  ProdMatrixExpr (TA A, TB B) : A_(A), B_(B) { }
  auto operator() (size_t i, size_t j) const {
    auto entry = 0;
    for (size_t k = 0; k < A_.width(); k++){
      entry += A_(i, k) * B_(k, j);
    }
    return entry;
  }
  size_t height() const { return A_.height(); }
  size_t width() const { return B_.width(); }     
};

template <typename TA, typename TB>
auto operator* (const MatrixExpr<TA> & A, const MatrixExpr<TB> & B)
{
  return ProdMatrixExpr(A.Upcast(), B.Upcast());
}

// Matrix * Vektor
template <typename TA, typename TB>
class ProdMatVecExpr : public VecExpr<ProdMatVecExpr<TA,TB>>
{
  TA A_;
  TB b_;
public:
  ProdMatrixExpr (TA A, TB b) : A_(A), b_(b) { ; }
  auto operator() (size_t i) const {
    auto entry = 0;
    for (size_t k = 0; k < A_.width(); k++){
      entry += A_(i, k) * b_(k);
    }
    return entry;
  }
  size_t Size() const { return A_.height(); }     
};

template <typename TA, typename TB>
auto operator* (const MatrixExpr<TA> & A, const VecExpr<TB> & b)
{
  return ProdMatVecExpr(A.Upcast(), b.Upcast());
}

  // TODO SumMatrixExpr *DONE:))*
  // TODO MatrixProductExpr *DONE:))*
  // TODO MatrixVectorProductExpr 


template <typename T>
std::ostream & operator<< (std::ostream & ost, const MatrixExpr<T> & A){
  if ((A.width() > 0) && (A.height() > 0)){
    for (size_t i = 0; i < A.height(); i++){
      ost << "(";
      for (size_t j = 0; j < A.width(); j++){
        // matrix elements are separated by tabs
        ost << A(i, j) << "\t";
      }
      ost << ")" << std::endl;
    }
  }else{
    ost << "empty matrix";
  }
  ost << std::endl;
  return ost;
}

} // namespace ASC_bla
 
#endif