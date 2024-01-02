===================
Matrix computations
===================

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

    .. code-block:: cpp

        Matrix<float, ColMajor> A(3000, 700);
        MatrixView<float, ColMajor> viewA(A.height(), A.width(), A.Data());

.. cpp:function:: MatrixView(size_t height, size_t width, T *data)

    Constructor that creates a view of a matrix given its dimensions, data pointer, and optional distance between elements.

.. cpp:function:: MatrixView(size_t height, size_t width, size_t dist, T *data)

    Constructor with an additional parameter for specifying the distance between elements.

The constructors create a view of the matrix given its dimensions, data pointer, and optional distance between elements. MatrixView operates as a lightweight wrapper around existing data.

T is the data type of the matrix elements, and ORD specifies whether the matrix is in RowMajor or ColMajor order.

.. admonition:: Important Note
    :class: warning

    MatrixView does not own the data; it merely provides a convenient way to access and manipulate the existing matrix.

.. cpp:function:: MatrixView(const MatrixView<T, ORD> & A)

    Copy constructor that creates a new MatrixView that shares the same data as the original, allowing for efficient and memory-safe matrix operations.

.. cpp:function:: MatrixView &operator=(const MatrixView & M)

    Assignment operator that sets the values of the current MatrixView to those of another MatrixView. The matrices must have the same dimensions.

.. cpp:function:: MatrixView &operator=(const MatrixExpr<TB> & M)

    Assignment operator that allows assigning the values of a MatrixExpr (such as another Matrix or MatrixView) to the current MatrixView.

.. cpp:function:: MatrixView &operator+=(const MatrixExpr<TB> & M)

    Compound assignment operator that adds the values of a MatrixExpr to the current MatrixView. The matrices must have the same dimensions.

.. cpp:function:: MatrixView &operator=(T scal)

    Set all elements of the MatrixView to a scalar value.

.. cpp:function:: MatrixView &operator*=(T scal)

    Multiply all elements of the MatrixView by a scalar value.

.. cpp:function:: auto View() const

    Returns a new MatrixView to the current object, allowing for further manipulations.

.. cpp:function:: size_t height() const
.. cpp:function:: size_t width() const

    Returns the dimensions of the matrix.

.. cpp:function:: T* Data()

    Returns a pointer to the underlying data of the matrix.

.. cpp:function:: size_t & Dist()

    Returns the distance in the data\_ array to the element next to/underneath it (depending on whether it is RowMajor (underneath) or ColMajor (next to)).

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



Fast matrix multiplication
--------------------------

The Neo-CLA library comprises functionality to compute matrix products more quickly.
The source code for this can be found in src/fastmult.hpp, with tests in tests/test_fastmult.cc.

.. cpp:function:: template <bool TIMED = false, typename BH = std::integral_constant<size_t, 96>, typename BW = std::integral_constant<size_t, 96>, ORDERING ORD> \
    void multparallel(MatrixView<double, RowMajor> C, MatrixView<double, ORD> A, MatrixView<double, RowMajor> B)

    This is the most important function of this chapter. It computes A*B and **adds** the product to C.
    TIMED specifies whether or not a pajéfile shall be created to log the timing of different tasks.
    BH and BW specify the height and width of the blocks of A that are extracted for blockwise multiplication.
    Adjusting these two parameters helps parts of A and B stay in Cache, see `the theory <https://jschoeberl.github.io/IntroSC/performance/caches1.html#cache-optimized-matrix-matrix-multiplication>`_.
    multparallel takes advantage of **SIMD, pipelining, caching and parallelization**.
    Note the restrictions on the Matrix ordering! Computing the product might require transposing a few matrices.

    .. code-block:: C++

        multparallel(C, A, B); // should be as easy as that

By constrast, multcachy lacks parallelization and multmatmat also lacks caching.
They are experimental predecessors in an evolution towards multparallel.


MatrixExpr
----------

Matrix expressions provide a powerful mechanism for composing complex operations on matrices. These enable concise and efficient representation of mathematical operations involving matrices.

It is composed of three primary types: SumMatrixExpr, ProdMatrixExpr, and ProdScalMatExpr, which represent addition, matrix multiplication, and scalar multiplication, respectively.

Addition (SumMatrixExpr)

Element-wise addition of two matrices. It ensures that the matrices being added have compatible dimensions.

.. cpp:function:: template <typename TA, typename TB> \
    auto operator+ (const MatrixExpr<TA> & A, const MatrixExpr<TB> & B)

Multiplication (ProdMatrixExpr)

Matrix multiplication. It ensures that the number of columns in the first matrix matches the number of rows in the second matrix.

.. cpp:function:: template <typename TA, typename TB> \
    auto operator* (const MatrixExpr<TA> & A, const MatrixExpr<TB> & B)

Scalar Multiplication (ProdScalMatExpr)

Multiplication of a matrix by a scalar. It allows scaling each element of the matrix by the specified scalar.

.. cpp:function:: template <typename TSCAL, typename TMAT> \
    auto operator* (double scal, const MatrixExpr<TMAT> & A)

Matrix-Vector Product (ProdMatVecExpr)

Multiplication of a matrix by a vector. It ensures that the number of columns in the matrix matches the size of the vector.

.. cpp:function:: template <typename TA, typename TB> \
    auto operator* (const MatrixExpr<TA> & A, const VectorExpr<TB> & b)

These expressions enhance the expressiveness and efficiency of matrix computations.

