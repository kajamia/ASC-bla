#ifndef FILE_MATRIX_H
#define FILE_MATRIX_H

#include <iostream>
#include <initializer_list>

#include "vector.h"
#include "matrix_expression.h"

namespace ASC_bla {

// choice of row or column major, for template
enum ORDERING { RowMajor, ColMajor };

template <typename T, ORDERING ORD>
class MatrixView : public MatrixExpr<MatrixView<T, ORD> >
{
 protected:
  size_t height_, width_, dist_;
  T *data_;

  // (TODO operator= for scalars)
  // TODO View()
  // TODO Row()
  // TODO Col()
  // TODO Rows()
  // TODO Cols()
  // TODO Transpose(MatrixView) (outside of MatrixView)
  // TODO (MatrixView) (outside of MatrixView)

 public:
  // constructor
  MatrixView(size_t height, size_t width, T *data)
    : height_(height), width_(width), data_(data) {
    if (ORD == RowMajor){
      dist_ = width;
    }else{
      dist_ = height;
    }
    }
  
  // copy constructor, for good measure
  MatrixView(const MatrixView<T, ORD> & A)
    : height_(A.height_), width_(A.width_), dist_(A.dist_), data_(A.data_) {;}
  

  // assignment operator
  template <typename TB>
  MatrixView &operator=(const MatrixExpr<TB> &M) {
    for (size_t i = 0; i < height_; i++) {
      for (size_t j = 0; j < width_; j++) {
        if (ORD == RowMajor) {
          data_[dist_ * i + j] = M(i, j);
        } else {
          data_[dist_ * j + i] = M(i, j);
        }
      }
    }
    return *this;
  }

/*
  MatrixView &operator=(T scal) {
    for (size_t i = 0; i < height_; i++) {
      for (size_t j = 0; j < width_; j++) {
        if (ORD == RowMajor) {
          data_[dist_ * (i * height_ + j)] = scal;
        } else {
          data_[dist_ * (j * width_ + i)] = scal;
        }
      }
    }
    return *this;
  }

  auto View() const { return MatrixView(height_, width_, dist_, data_); }*/
  size_t height() const { return this->height_; };
  size_t width() const { return this->width_; };

  // round bracket access operator
  T &operator()(size_t i, size_t j) {
    if (ORD == RowMajor) {
      return data_[i * dist_ + j];
    }
    if (ORD == ColMajor) {
      return data_[j * dist_ + i];
    }
  }

  // round bracket access operator, constant version
  const T &operator()(size_t i, size_t j) const {
    if (ORD == RowMajor) {
      return data_[i * dist_ + j];
    }
    if (ORD == ColMajor) {
      return data_[j * dist_ + i];
    }
  }

};

template <typename T, ORDERING ORD>
class Matrix : public MatrixView<T, ORD> {
  typedef MatrixView<T, ORD> BASE;
  using BASE::data_;
  using BASE::height_;
  using BASE::width_;
  using BASE::dist_;

  // (TODO constructor from MatrixExpr)
  // TODO move operator=
  // optional: constructor from initializer list

 public:
  // constructor
  Matrix(size_t height, size_t width)
    : MatrixView<T, ORD> (height, width, new T[height*width]) {;}

  // copy constructor
  Matrix (const Matrix & A)
    : Matrix(A.height(), A.width())
  {
    *this=A;
  }

  // move constructor
  Matrix (Matrix && A)
      : MatrixView<T, ORD> (0, 0, nullptr)
    {
      std::swap(width_, A.width_);
      std::swap(height_, A.height_);
      std::swap(data_, A.data_);
    }
  
  // initializer list constructor
  Matrix (size_t width, size_t height, std::initializer_list<T> list)
    : MatrixView<T, ORD> (height, width, new T[list.size()]) {
    // check if list has the right size
    if (list.size() != height_*width_){
      throw list.size();
      std::cout << "err";
      return;
    }else{
      // copy list
      for (size_t i = 0; i < list.size(); i++){
        data_[i] = list.begin()[i];
      }
    }

    // set dist
    if (ORD == RowMajor){
      dist_ = width;
    }else{
      dist_ = height;
    }
  }

  // destructor
  ~Matrix () { delete [] data_; }

  // assignment operator
  using BASE::operator=;
  Matrix & operator=(const Matrix & A2)
  {
    // matrices need to have the same dimensions!
    if (height_ != A2.height()){
      throw A2.height();
      return *this;
    }else if (width_ != A2.width()){
      throw A2.width();
      return *this;
    }

    // setting
    for (size_t i = 0; i < height_; i++){
      for (size_t j = 0; j < width_; j++){
        if (ORD == RowMajor) {
          data_[i*width_ + j] = A2(i, j);
        } else {
          data_[j*height_ + i] = A2(i, j);
        }
      }
    } 
    return *this;
  }

};

// output stream operator (without variadic templates)
template <typename T, ORDERING ORD>
std::ostream & operator<< (std::ostream & ost, const MatrixView<T, ORD> & A){
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


}  // namespace ASC_bla

#endif
