#include <sstream>
#include <pybind11/pybind11.h>

#include "vector.h"
#include "matrix.h"

using namespace ASC_bla;
namespace py = pybind11;




PYBIND11_MODULE(bla, m) {
    m.doc() = "Basic linear algebra module"; // optional module docstring
    
    py::class_<Vector<double>> (m, "Vector")
      .def(py::init<size_t>(),
           py::arg("size"), "create vector of given size")
      .def("__len__", &Vector<double>::Size,
           "return size of vector")
      
      .def("__setitem__", [](Vector<double> & self, int i, double v) {
        if (i < 0) i += self.Size();
        if (i < 0 || i >= self.Size()) throw py::index_error("vector index out of range");
        self(i) = v;
      })
      .def("__getitem__", [](Vector<double> & self, int i) { return self(i); })
      
      .def("__setitem__", [](Vector<double> & self, py::slice inds, double val)
      {
        size_t start, stop, step, n;
        if (!inds.compute(self.Size(), &start, &stop, &step, &n))
          throw py::error_already_set();
        self.Range(start, stop).Slice(0,step) = val;
      })
      
      .def("__add__", [](Vector<double> & self, Vector<double> & other)
      { return Vector<double> (self+other); })

      .def("__rmul__", [](Vector<double> & self, double scal)
      { return Vector<double> (scal*self); })
      
      .def("__str__", [](const Vector<double> & self)
      {
        std::stringstream str;
        str << self;
        return str.str();
      })

     .def(py::pickle(
        [](Vector<double> & self) { // __getstate__
            /* return a tuple that fully encodes the state of the object */
          return py::make_tuple(self.Size(),
                                py::bytes((char*)(void*)&self(0), self.Size()*sizeof(double)));
        },
        [](py::tuple t) { // __setstate__
          if (t.size() != 2)
            throw std::runtime_error("should be a 2-tuple!");

          Vector<double> v(t[0].cast<size_t>());
          py::bytes mem = t[1].cast<py::bytes>();
          std::memcpy(&v(0), PYBIND11_BYTES_AS_STRING(mem.ptr()), v.Size()*sizeof(double));
          return v;
        }))
    ;


    // matrix class
    py::class_<Matrix<double> > (m, "Matrix")
      .def(py::init<size_t, size_t>(),
        py::arg("height"), py::arg("width"), "create empty matrix")
      .def("__getitem__",
        [](Matrix<double>& self, std::tuple<int, int> ind) {
          return self(std::get<0>(ind), std::get<1>(ind));
      })
      .def_property_readonly("shape",
        [](const Matrix<double>& self) {
          return std::tuple(self.height(), self.width());
      })
      .def("__setitem__", [](Matrix<double>& self, std::tuple<int, int> ind, double e){
        size_t i = std::get<0>(ind);
        size_t j = std::get<1>(ind);
        if (i < 0){i += self.height();}
        if (j < 0){j += self.width();}
        if (i < 0 || j < 0 || i >= self.height() || j >= self.width()){ throw py::index_error("index out of range");}

        self(i, j) = e;
      })
      .def("__add__", [](const Matrix<double>& self, const Matrix<double>& other){
        return Matrix<double> (self + other);
      })
      .def("__mul__", [](const Matrix<double>& self, const Matrix<double>& other){
        return Matrix<double> (self * other);
      })
      .def("__mul__", [](const Matrix<double>& self, const Vector<double>& other){
        return Vector<double> (self * other);
      })
      .def("__str__", [](const Matrix<double> & self)
      {
        std::stringstream str;
        str << self;
        return str.str();
      })
    ;
}
