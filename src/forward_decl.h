#ifndef FILE_FWDDECL_H
#define FILE_FWDDECL_H

namespace Neo_CLA{

  // choice of row or column major, for template
  enum ORDERING { RowMajor, ColMajor };


  template <typename T = double, typename TDIST = std::integral_constant<size_t,1> >
  class VectorView;


  template <typename T = double, ORDERING ORD = RowMajor>
  class MatrixView;
}

#endif
