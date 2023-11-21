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

MatrixExpr
----------
