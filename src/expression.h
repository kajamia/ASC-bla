#ifndef FILE_EXPRESSION_H
#define FILE_EXPRESSION_H


namespace Neo_CLA
{

  template <typename T>
  class VectorExpr
  {
   protected:
    // the following two are to prevent copying of VectorExpr-objects
    VectorExpr() = default;
    VectorExpr(const VectorExpr & v) = default;
   public:
    auto Upcast() const { return static_cast<const T&> (*this); }
    size_t Size() const { return Upcast().Size(); }
    auto operator() (size_t i) const { return Upcast()(i); }
  };
  
 
  template <typename TA, typename TB>
  class SumVecExpr : public VectorExpr<SumVecExpr<TA,TB>>
  {
    TA a_;
    TB b_;
  public:
    SumVecExpr (TA a, TB b) : a_(a), b_(b) { }

    auto operator() (size_t i) const { return a_(i)+b_(i); }
    size_t Size() const { return a_.Size(); }      
  };
  
  template <typename TA, typename TB>
  auto operator+ (const VectorExpr<TA> & a, const VectorExpr<TB> & b)
  {
    return SumVecExpr(a.Upcast(), b.Upcast());
  }


  template <typename TA, typename TB>
  class DifferenceVecExpr : public VectorExpr<DifferenceVecExpr<TA,TB>>
  {
    TA a_;
    TB b_;
  public:
    DifferenceVecExpr (TA a, TB b) : a_(a), b_(b) { }

    auto operator() (size_t i) const { return a_(i)-b_(i); }
    size_t Size() const { return a_.Size(); }      
  };
  
  template <typename TA, typename TB>
  auto operator- (const VectorExpr<TA> & a, const VectorExpr<TB> & b)
  {
    return DifferenceVecExpr(a.Upcast(), b.Upcast());
  }



  
  template <typename TSCAL, typename TV>
  class ScaleVecExpr : public VectorExpr<ScaleVecExpr<TSCAL,TV>>
  {
    TSCAL scal_;
    TV vec_;
  public:
    ScaleVecExpr (TSCAL scal, TV vec) : scal_(scal), vec_(vec) { }

    auto operator() (size_t i) const { return scal_*vec_(i); }
    size_t Size() const { return vec_.Size(); }      
  };
  
  template <typename T>
  auto operator* (double scal, const VectorExpr<T> & v)
  {
    return ScaleVecExpr(scal, v.Upcast());
  }



  template <typename T>
  std::ostream & operator<< (std::ostream & ost, const VectorExpr<T> & v)
  {
    if (v.Size() > 0)
      ost << v(0);
    for (size_t i = 1; i < v.Size(); i++)
      ost << ", " << v(i);
    return ost;
  }


  // scalar product
  template <typename T1, typename T2>
  auto operator* (const VectorExpr<T1> & v1, const VectorExpr<T2> & v2){
    // error handling
    if (v1.Size() != v2.Size()){
      throw std::invalid_argument("vectors need to have same length for scalar product");
    }

    decltype(v1(0)*v2(0)) product = 0;

    for (size_t i = 0; i < v1.Size(); i++){
      product += v1(i)*v2(i);
    }

    return product;
  }

  // 2-norm for vectors
  template <typename T>
  auto L2Norm (const VectorExpr<T> & v){
    return std::sqrt(v*v);
  }
  
}
 
#endif
