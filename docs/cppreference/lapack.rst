================
Lapack interface
================

Some functionality has been outsourced to Lapack.

.. cpp:function:: void MultMatMatLapack (MatrixView<double, OA> a, \
                         MatrixView<double, OB> b, \
                         MatrixView<double, OC> c)

    writes a*b to c

.. cpp:class:: template<ORDERING ORD> \
                LapackLU

    .. cpp:function:: LapackLU (Matrix<double, ORD> A)


    creates an LU decomposition and provides functionality on it:

    .. cpp:function:: void Solve (VectorView<double> b)

        solves the system of linear equations Ax=b; the solution is written to the storage of b

    .. cpp:function:: Matrix<double,ColMajor> Inverse()

        inverse of A

    .. cpp:function:: Matrix<double, ColMajor> PFactor() const
    .. cpp:function:: Matrix<double, ColMajor> LFactor() const
    .. cpp:function:: Matrix<double, ColMajor> UFactor() const

        copies the P, L and U matrices to separate matrices
    