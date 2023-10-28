#ifndef FILE_MATRIX_H
#define FILE_MATRIX_H

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

  // TODO constructor (with implementation of dist inside!)
  // TODO copy constructor
  // TODO operator=
  // (TODO operator= for scalars)
  // TODO View()
  // TODO width(), height()
  // TODO operator() (as & and as const &)
  // TODO Row()
  // TODO Col()
  // TODO Rows()
  // TODO Cols()
  // TODO Transpose(MatrixView) (outside of MatrixView)
  // TODO (MatrixView) (outside of MatrixView)
/*
 public:
  // constr
  MatrixView(size_t height, size_t width, size_t dist; T *data)
      : height_(height), width_(width), dist_(dist); data_(data) {}

  // constr für slice etc.
  MatrixView(size_t height, size_t width, size_t dist, T *data)
      : height_(height), width_(width), dist_(dist), data_(data) {}

  template <typename TB>
  MatrixView &operator=(const MatrixExpr<TB> &M) {
    for (size_t i = 0; i < height_; i++) {
      for (size_t j = 0; j < width_; j++) {
        if (ORD == RowMajor) {
          data_[dist_ * (i * height_ + j)] = M(i, j);
        } else {
          data_[dist_ * (j * width_ + i)] = M(i, j);
        }
      }
    }
    return *this;
  }

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

  auto View() const { return MatrixView(height_, width_, dist_, data_); }
  size_t height() const { return this->height_; };
  size_t width() const { return this->width_; };
  T &operator()(size_t i) { return data_[dist_ * i]; }
  const T &operator()(size_t i) const { return data_[dist_ * i]; }

  // round bracket access operator
  const T &operator()(size_t i, size_t j) const {
    if (ORD == RowMajor) {
      return data_[i * dist_ + j];
    }
    if (ORD == ColMajor) {
      return data_[j * dist_ + i];
    }
  }

  T &operator()(size_t i, size_t j) {
    if (ORD == RowMajor) {
      return data_[i * dist_ + j];
    }
    if (ORD == ColMajor) {
      return data_[j * dist_ + i];
    }
  }*/
};

template <typename T, ORDERING ORD>
class Matrix : public MatrixView<T, ORD> {
  typedef MatrixView<T, ORD> BASE;
  using BASE::data_;
  using BASE::height_;
  using BASE::dist_;
  using BASE::width_;

  // (TODO move constructor)
  // (TODO expression constructor)
  // TODO destructor
  // TODO operator=
  // TODO move operator=
  // TODO output stream operator
/*
 public:
  Matrix(size_t height_, size_t width_)
    : MatrixView<T, ORD> (size, new T[data]) {
      if (ORD == RowMajor){
        dist_ = width_;
      }else{
        dist_ = height;
      }
    }

  Matrix (const Matrix & A)
    : Matrix(A.height(), A.width())
  {
    *this=A;

    if (ORD == RowMajor){
        dist_ = width_;
      }else{
        dist_ = height;
      }
  }

  size_t height() const { return this->height_; };
  size_t width() const { return this->width_; };

  Matrix (Matrix && A)
      : MatrixView<T, ORD> (0, 0, nullptr)
    {
      std::swap(width_, v.width_);
      std::swap(height_, v.height_);
      std::swap(data_, v.data_);
    }

  ~Matrix () { delete [] data_; }

  using BASE::operator=;
  Matrix & operator=(const Matrix & A2)
  {
    for (size_t i = 0; i < height_; i++){
      for (size_t j = 0; j < width_; j++){
        data_[i] = A2(i, j);
      }
    } 
    return *this;
  }

  Matrix & operator=(Matrix & A2)
  {
    for (size_t i = 0; i < height_; i++){
      for (size_t j = 0; j < width_; j++){
        data_[i] = A2(i, j);
      }
    } 
    return *this;
  }*/
};


}  // namespace ASC_bla

#endif
