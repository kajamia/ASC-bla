#ifndef FILE_VECTOR_H
#define FILE_VECTOR_H

#include <exception>
#include <iostream>
#include <cmath>


#include "expression.h"


namespace Neo_CLA
{
   
  template <typename T = double, typename TDIST = std::integral_constant<size_t,1> >
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
    
    template <typename TB>
    VectorView & operator= (const VectorView<TB> & v2)
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

  // scalar product
  template <typename T1, typename T2, typename TDIST1, typename TDIST2>
  auto operator* (VectorView<T1, TDIST1> v1, VectorView<T2, TDIST2> v2){
    // error handling
    if (v1.Size() != v2.Size()){
      throw std::invalid_argument("vectors need to have same length for scalar product");
    }

    decltype(T1(0)*T2(0)) product = 0;

    for (size_t i = 0; i < v1.Size(); i++){
      product += v1(i)*v2(i);
    }

    return product;
  }

  // 2-norm for vectors
  template <typename T, typename TDIST>
  auto L2Norm (VectorView<T, TDIST> v){
    return std::sqrt(v*v);
  }

  template <typename ...Args>
  std::ostream & operator<< (std::ostream & ost, const VectorView<Args...> & v)
  {
    if (v.Size() > 0)
      ost << v(0);
    for (size_t i = 1; i < v.Size(); i++)
      ost << ", " << v(i);
    return ost;
  }
  
}

#endif
