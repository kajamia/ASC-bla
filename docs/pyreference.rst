====================
Python API reference
====================

The objects below are within the Neosoft.cla module.

Vector
======


Matrix
======

.. class:: class Neosoft.cla.Matrix(height, width)

    Initially, the matrix is empty (random values).

    .. code-block::

        >>> A = Matrix(m, n)
        >>> A(3, 4) = 5
        >>> print(A(3, 4))
        5

    The class also supports addition and multiplication with other matrices, vectors and scalars.

    .. code-block::

        A + B*v + 3*C + D*5

    Apart from that, Matrix is compatible with pickling and buffering.

    .. method:: Matrix.shape()

        returns the tuple (height, width)

    .. method:: Matrix.Data()

        returns the internal data pointer of the matrix
