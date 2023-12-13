#ifndef FILE_VECTOR_H
#define FILE_VECTOR_H

#include <exception>
#include <iostream>
#include <cmath>


#include "forward_decl.h"
#include "expression.h"


namespace Neo_CLA
{
   
  template <typename T, typename TDIST>
  class VectorView : public VecExpr<VectorView<T,TDIST>>
  {
  protected:
    T * data_;
    size_t size_;
    TDIST dist_;
  public:
    VectorView (size_t size, T * data)
      : data_(data), size_(size) { }
    
    VectorView (size_t size, TDIST dist, T * data)
      : data_(data), size_(size), dist_(dist) { }

    VectorView(const VectorView<T, TDIST> & V)
      : data_(V.data_), size_(V.size_), dist_(V.dist_) { }
    
    /* VectorView(const VecExpr<T> & V)
      : data_(V.Size()), size_(V.Size()) {
        for (size_t i=0; i < size_; i++){
          data_[i] = V(i);
        }
      } */
    
    VectorView & operator= (const VectorView & v2)
    {
      for (size_t i = 0; i < size_; i++)
        data_[dist_*i] = v2(i);
      return *this;
    }

    template <typename TB>
    VectorView & operator= (const VecExpr<TB> & v2)
    {
      for (size_t i = 0; i < size_; i++)
        data_[dist_*i] = v2(i);
      return *this;
    }

    template <typename TB>
    VectorView & operator= (std::initializer_list<TB> list)
    {
      for (size_t i = 0; i < size_; i++)
        data_[dist_*i] = list.begin()[i];
      return *this;
    }

    template <typename TB>
    VectorView & operator+= (const VecExpr<TB> & v2)
    {
      for (size_t i = 0; i < size_; i++)
        data_[dist_*i] += v2(i);
      return *this;
    }

    template <typename TB>
    VectorView & operator-= (const VecExpr<TB> & v2)
    {
      for (size_t i = 0; i < size_; i++)
        data_[dist_*i] -= v2(i);
      return *this;
    }

    VectorView & operator= (T scal)
    {
      for (size_t i = 0; i < size_; i++)
        data_[dist_*i] = scal;
      return *this;
    }

    VectorView & operator*= (T scal)
    {
      for (size_t i = 0; i < size_; i++)
        data_[dist_*i] *= scal;
      return *this;
    }

    VectorView & operator/= (T scal)
    {
      for (size_t i = 0; i < size_; i++)
        data_[dist_*i] /= scal;
      return *this;
    }
    
    auto View() const { return VectorView(size_, dist_, data_); }
    size_t Size() const { return size_; }
    T* Data() { return data_; }
    const size_t Dist() { return dist_; }
    T & operator()(size_t i) { return data_[dist_*i]; }
    const T & operator()(size_t i) const { return data_[dist_*i]; }
    
    auto Range(size_t first, size_t next) const {
      return VectorView(next-first, dist_, data_+first*dist_);
    }

    auto Slice(size_t first, size_t slice) const {
      return VectorView<T,size_t> (size_/slice, dist_*slice, data_+first*dist_);
    }

    auto AsMatrix (size_t m, size_t n){
      static_assert(std::is_same<TDIST, std::integral_constant<size_t,1> >::value == true, "gapped vectors cannot be converted to matrices");
      return MatrixView<T, RowMajor> (m, n, data_);
    }

  };
  
  

  
  template <typename T = double>
  class Vector : public VectorView<T>
  {
    typedef VectorView<T> BASE;
    using BASE::size_;
    using BASE::data_;
  public:
    Vector (size_t size) 
      : VectorView<T> (size, new T[size]) { ; }
    
    Vector (const Vector & v)
      : Vector(v.Size())
    {
      *this = v;
    }

    Vector (Vector && v)
      : VectorView<T> (0, nullptr)
    {
      std::swap(size_, v.size_);
      std::swap(data_, v.data_);
    }

    template <typename TB>
    Vector (const VecExpr<TB> & v)
      : Vector(v.Size())
    {
      *this = v;
    }

    // initializer list constructor
    Vector (std::initializer_list<T> list)
    : VectorView<T> (list.size(), new T[list.size()]) {
    // copy list
    for (size_t i = 0; i < list.size(); i++){
      data_[i] = list.begin()[i];
    }
    }
    
    
    ~Vector () { delete [] data_; }

    using BASE::operator=;
    Vector & operator=(const Vector & v2)
    {
      for (size_t i = 0; i < size_; i++)
        data_[i] = v2(i);
      return *this;
    }

    Vector & operator= (Vector && v2)
    {
      for (size_t i = 0; i < size_; i++)
        data_[i] = v2(i);
      return *this;
    }
    
  };

  

  template <typename ...Args>
  std::ostream & operator<< (std::ostream & ost, const VectorView<Args...> & v)
  {
    if (v.Size() > 0)
      ost << v(0);
    for (size_t i = 1; i < v.Size(); i++)
      ost << ", " << v(i);
    return ost;
  }


  // fixed-size vector
  template <int SIZE, typename T = double>
  class Vec : public VecExpr<Vec<SIZE,T>>
  {
  T data[SIZE];

   public:
    Vec (){};

    Vec (const Vec<SIZE, T> & v2){
      for (int i=0; i < SIZE; i++){
        data[i] = v2(i);
      }
    }

    Vec (T all){
      for (size_t i=0; i < SIZE; i++){
        data[i] = all;
      }
    };

    // initializer list constructor
    Vec (std::initializer_list<T> list){
      if (list.size() != SIZE) throw std::invalid_argument("initializer list with wrong size");
      // copy list
      for (size_t i = 0; i < SIZE; i++){
        data[i] = list.begin()[i];
    }
    }

    // constructor from VectorView
    template<typename T2, typename TDIST>
    Vec (VectorView<T2, TDIST> v2){
      if (v2.Size() != SIZE) throw std::invalid_argument("VectorView has wrong size");
      // copy list
      for (size_t i=0; i < SIZE; i++){
        data[i] = v2(i);
      }
    }

    // constructor from VecExpr
    template<typename T2>
    Vec (const VecExpr<T2> & v2){
      if (v2.Size() != SIZE) throw std::invalid_argument("VecExpr has wrong size");

      for (int i=0; i < SIZE; i++){
        data[i] = v2(i);
      }
    }

    // VectorView operator=
    template<typename T2, typename TDIST>
    Vec & operator= (VectorView<T2, TDIST> v2){
      if (v2.Size() != SIZE) throw std::invalid_argument("VectorView has wrong size");
      // copy list
      for (size_t i=0; i < SIZE; i++){
        data[i] = v2(i);
      }
      return *this;
    }

    Vec & operator= (const Vec<SIZE, T> & v2){
      for (int i=0; i < SIZE; i++){
        data[i] = v2(i);
      }
      return *this;
    }

    template<typename T2>
    Vec & operator= (const VecExpr<T2> & v2){
      if (v2.Size() != SIZE) throw std::invalid_argument("VecExpr has wrong size");

      for (int i=0; i < SIZE; i++){
        data[i] = v2(i);
      }
      return *this;
    }

    T operator() (size_t i) const{
      return data[i];
    }

    T & operator() (size_t i){
      return data[i];
    }

  };
  
}

#endif
