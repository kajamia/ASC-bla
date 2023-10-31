#ifndef FILE_MATRIX_EXPRESSION_H
#define FILE_MATRIX_EXPRESSION_H

#include "matrix.h"

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

  // TODO SumMatrixExpr
  // TODO MatrixProductExpr
  // TODO MatrixVectorProductExpr


template <typename T>
std::ostream & operator<< (std::ostream & ost, const MatrixExpr<T> & A){
  if ((A.width() > 0) && (A.height() > 0)){
    for (size_t j = 0; j < A.width(); j++){
      ost << "(";
      for (size_t i = 0; i < A.height(); i++){
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