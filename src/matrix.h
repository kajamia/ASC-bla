#ifndef FILE_MATRIX_H
#define FILE_MATRIX_H

#include <vector.h>

namespace ASC_bla {

// choice of row or column major, for template
enum ORDERING { RowMajor, ColMajor };

template <typename T, template ORDERING>
class MatrixView {
protected:
  size_t height_, width_, dist_;
  T *data_;

public:
  //constr
  MatrixView(size_t height, size_t width, T* data)
    : height_(height), width_(width), data_(data) {}

  //constr für slice etc.
  MatrixView(size_t height, size_t width, size_t dist, T* data)
    : height_(height), width_(width), dist_(dist), data_(data) {}

  template <typename TB>
  MatrixView & operator= (const MatrixExpr<TB> & M)
  {
    for (size_t i = 0; i < height_; i++){
      for(size_t j = 0; j < width_; j++){
        if (ORDERING==RowMajor){
          data_[dist_*(i*height_+j)] = M(i,j);
        }
        else{
          data_[dist_*(j*width_+i)] = M(i,j);
        }        
      }
    } 
    return *this;
  }

  MatrixView & operator= (T scal)
  {
  for (size_t i = 0; i < height_; i++){
        for(size_t j = 0; j < width_; j++){
          if (ORDERING==RowMajor){
            data_[dist_*(i*height_+j)] = scal;
          }
          else{
            data_[dist_*(j*width_+i)] = scal;
          }        
        }
      } 
      return *this;
  }

  auto View() const { return MatrixView(height_, width_, dist_, data_); }
  size_t height() const { return this->height_; };
  size_t width() const { return this->width_; };
  T & operator()(size_t i) { return data_[dist_*i]; }
  const T & operator()(size_t i) const { return data_[dist_*i]; }


};

template <typename T, ORDERING ORD>
class Matrix {
  size_t height_;
  size_t width_;
  T *data_;

 public:
  Matrix(size_t height, size_t width)
      : height_(height), width_(width), data_(new T[height * width]){};

  Matrix(const Matrix &A) : Matrix(A.height(), A.width()) { *this = A; };

  // TODO move ctor

  ~Matrix() { delete[] data_; }

  Matrix &operator=(const Matrix &A2) {
    for (size_t i = 0; i < height_; i++) {
      for (size_t j = 0; j < width_; j++) {
        (*this)(i, j) = A2(i, j);
      };
    }
    return *this;
  }
  // TODO move operator=

  size_t height() const { return this->height_; };
  size_t width() const { return this->width_; };

  // round bracket access operator
  const T &operator()(size_t i, size_t j) const {
    if (ORD == RowMajor) {
      return data_[i * width_ + j];
    }
    if (ORD == ColMajor) {
      return data_[j * height_ + i];
    }
  }

  T &operator()(size_t i, size_t j) {
    if (ORD == RowMajor) {
      return data_[i * width_ + j];
    }
    if (ORD == ColMajor) {
      return data_[j * height_ + i];
    }
  }
};

Vector operator*(Matrix &M, Vector &v) const {
  Vector<double> product(n);  // strichpunkte
  size_t n = M.height();
  size_t m = M.width();
  double temp = 0;

  if (m != v.Size()) {
    break;
  }

  for (size_t i = 0, i < n, i++) {  // klammern???
    for (size_t j = 0, j < m, j++) {
      temp += (M(i, j) * v(j));  // klammer notwendig?
    }
    product(i) = temp;
    temp = 0;
  }

  return product;
}

Matrix operator*(Matrix &A, Matrix &B) const {
  Matrix<double> Product(n, l);  // strichpunkte
  size_t n = A.height();
  size_t m = A.width();
  size_t l = B.width();
  double temp = 0;

  if (m != B.height()) {
    break;
  }

  for (size_t i = 0, i < l, i++) {
    for (size_t j = 0, j < n, j++) {  // klammern???
      for (size_t k = 0, k < m, k++) {
        temp += (A(j, k) * B(k, i));  // klammer notwendig?
      }
      Product(j, i) = temp;
      temp = 0;
    }
  }
  return Product;
}

Matrix transp(Matrix &A) {
  Matrix<double> Transp(m, n);
  size_t n = A.height();
  size_t m = A.width();

  for (size_t i = 0, i < n, i++) {
    for (size_t j = 0, j < m, j++) {
      Transp(j, i) = A(i, j);
    }
  }

  return Transp;
}
}  // namespace ASC_bla

#endif
