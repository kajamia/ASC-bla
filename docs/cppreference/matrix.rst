===================
Matrix computations
===================
..
    matrix sachen dokumentieren
    youtube/google: ("restructured text" und "sphinx")
    das dokumentieren was aufgabe war
    norbert will JEDEN code in blöcken haben

Matrix
------

This is the main class of this project.

.. cpp:class:: template <typename T = double, ORDERING ORD = RowMajor> \
    Matrix: public MatrixView<T, ORD>

    .. cpp:function:: Matrix (size_t height, size_t width)
    .. cpp:function:: Matrix (size_t height, size_t width, std::initializer_list<T> list)
    
    There is one constructor that creates an empty matrix of given dimensions.
    The initializer list constructor stores an initalizer list into a matrix.

    T is the data type of the elements of the matrix. You can use about any data type with Matrix.
    As for the ordering, **RowMajor** and **ColMajor** are available.

    .. code-block:: cpp

        Matrix<float, ColMajor> A(3000, 700); // 3000 rows, 700 columns, as float, Fortran-style
        Matrix B(4, 7, {1, 2, 3, 4, 5, 6, 7,
                        8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21,
                        22, 23, 24, 25, 26, 27, 28});
    
    .. admonition:: There is much more to matrix than that!
        :class: note

        Most of the features of Matrix are not actually provided by the Matrix class itself,
        but inherited from MatrixView and (indirectly) MatrixExpr.


Inverse
-------

.. cpp:function:: template <typename T, ORDERING ORD> \
    Matrix<T, ORD> Inverse (const Matrix<T, ORD> & M)

    This function computes the inverse of a Matrix
    (or anything that can be type-cast to a Matrix, such as MatrixView, MatrixExpr).
    It uses the pivot element method.

MatrixView
----------

MatrixView is a fundamental class that provides a view into a matrix. It inherits from MatrixExpr, allowing for expression-based matrix computations.
It provides a versatile interface for accessing and manipulating matrix data, forming the foundation for various matrix computations.

.. cpp:function:: MatrixView (size_t height, size_t width, T *data)
.. cpp:function:: MatrixView (size_t height, size_t width, size_t dist, T *data)

The constructors create a view into a matrix given its dimensions, data pointer, and optional distance between elements. MatrixView operates as a lightweight wrapper around existing data.

T is the data type of the matrix elements, and ORD specifies whether the matrix is in RowMajor or ColMajor order.

.. code-block:: cpp

    Matrix<float, ColMajor> A(3000, 700); // Create a matrix (ColMajor) with 3000 rows and 700 columns
    MatrixView<float, ColMajor> viewA(A.height(), A.width(), A.Data()); // Create a view into matrix A

.. admonition:: Important Note
    :class: warning

    MatrixView does not own the data; it merely provides a convenient way to access and manipulate the existing matrix.

.. cpp:function:: MatrixView(const MatrixView<T, ORD> & A)

The copy constructor creates a new MatrixView that shares the same data as the original, allowing for efficient and memory-safe matrix operations.

.. cpp:function:: MatrixView &operator=(const MatrixView & M)

The assignment operator sets the values of the current MatrixView to those of another MatrixView. The matrices must have the same dimensions.

.. cpp:function:: MatrixView &operator=(const MatrixExpr<TB> & M)

The assignment operator allows assigning the values of a MatrixExpr (such as another Matrix or MatrixView) to the current MatrixView.

.. cpp:function:: MatrixView &operator+=(const MatrixExpr<TB> & M)

The compound assignment operator adds the values of a MatrixExpr to the current MatrixView. The matrices must have the same dimensions.

.. cpp:function:: MatrixView &operator= (T scal)

Set all elements of the MatrixView to a scalar value.

.. cpp:function:: MatrixView &operator*= (T scal)

Multiply all elements of the MatrixView by a scalar value.

.. cpp:function:: auto View() const

Returns a new MatrixView to the current object, allowing for further manipulations.

.. cpp:function:: size_t height() const
.. cpp:function:: size_t width() const

Returns the dimensions of the matrix.

.. cpp:function:: T* Data()

Returns a pointer to the underlying data of the matrix.

.. cpp:function:: size_t & Dist()

Returns a reference to the distance between elements in the matrix.

.. cpp:function:: T &operator()(size_t i, size_t j)
.. cpp:function:: const T &operator()(size_t i, size_t j) const

Accesses the elements of the matrix using round brackets. The behavior depends on the ordering (RowMajor or ColMajor).

.. cpp:function:: auto transposed() const

Returns a transposed view of the matrix.

.. cpp:function:: auto Row(size_t i)
.. cpp:function:: auto Col(size_t j)

Returns a VectorView representing the i-th row or j-th column of the matrix.

.. cpp:function:: auto Diag()

Returns a VectorView representing the diagonal of the matrix.

.. cpp:function:: auto Rows(size_t start, size_t height)
.. cpp:function:: auto Cols(size_t start, size_t width)

Returns a MatrixView representing a submatrix obtained by selecting a range of rows or columns.

.. cpp:function:: void swapcols(size_t i, size_t j)

Swaps two columns of the matrix efficiently using row-wise swapping.


MatrixExpr
----------
