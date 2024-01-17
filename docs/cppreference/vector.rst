===================
Vector computations
===================

VectorView
-----------

This is the main class of this project for vector operations.

.. cpp:class:: template <typename T, typename TDIST> \
    VectorView: public VectorExpr<VectorView<T, TDIST>>

    .. cpp:function:: VectorView (size_t size, T * data)
    .. cpp:function:: VectorView (size_t size, TDIST dist, T * data)
    .. cpp:function:: VectorView (const VectorView<T, TDIST> & V)
    .. cpp:function:: VectorView & operator= (const VectorView & v2)

    There are constructors to create a vector view with given size and data. The assignment operator is overloaded to assign the values of one vector view to another.

    .. code-block:: cpp

        double data_array[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
        VectorView<double, std::integral_constant<size_t, 1>> V(5, data_array); // Vector view of size 5 with data_array

Vector
------

This class represents a vector and inherits from VectorView.

.. cpp:class:: template <typename T = double> \
    Vector: public VectorView<T>

    .. cpp:function:: Vector (size_t size)
    .. cpp:function:: Vector (const Vector & v)
    .. cpp:function:: Vector (Vector && v)
    .. cpp:function:: Vector (const VectorExpr<TB> & v)
    .. cpp:function:: Vector (std::initializer_list<T> list)
    .. cpp:function:: ~Vector ()
    .. cpp:function:: Vector & operator= (const Vector & v2)
    .. cpp:function:: Vector & operator= (Vector && v2)

    The constructors create a vector of given size or initialize it using an initializer list. The assignment operators allow copying and moving of vectors.

    .. code-block:: cpp

        Vector<double> A(5); // Vector of size 5
        Vector<double> B = {1.0, 2.0, 3.0, 4.0, 5.0}; // Vector initialized with an initializer list

Vec
---

This class represents a fixed-size vector.

.. cpp:class:: template <int SIZE, typename T = double> \
    Vec: public VectorExpr<Vec<SIZE, T>>

    .. cpp:function:: Vec ()
    .. cpp:function:: Vec (const Vec<SIZE, T> & v2)
    .. cpp:function:: Vec (T all)
    .. cpp:function:: Vec (std::initializer_list<T> list)
    .. cpp:function:: template<typename T2, typename TDIST> Vec (VectorView<T2, TDIST> v2)
    .. cpp:function:: template<typename T2> Vec (const VectorExpr<T2> & v2)
    .. cpp:function:: Vec & operator= (VectorView<T2, TDIST> v2)
    .. cpp:function:: Vec & operator= (const Vec<SIZE, T> & v2)
    .. cpp:function:: template<typename T2> Vec & operator= (const VectorExpr<T2> & v2)
    .. cpp:function:: T operator() (size_t i) const
    .. cpp:function:: T & operator() (size_t i)
    .. cpp:function:: size_t Size() const

    The constructors allow creating a fixed-size vector with various initialization options. The assignment operators enable assigning values from other vectors or expressions.

    .. code-block:: cpp

        Vec<3, double> C; // Fixed-size vector of size 3
        Vec<3, double> D = {1.0, 2.0, 3.0}; // Fixed-size vector initialized with an initializer list

Operators
---------

Scalar product and 2-norm operations are provided for vectors.

.. cpp:function:: template <typename T1, typename T2, typename TDIST1, typename TDIST2> \
    auto operator* (VectorView<T1, TDIST1> v1, VectorView<T2, TDIST2> v2)

    Computes the scalar product of two vectors.

.. cpp:function:: template <typename T, typename TDIST> \
    auto L2Norm (VectorView<T, TDIST> v)

    Computes the 2-norm of a vector.


