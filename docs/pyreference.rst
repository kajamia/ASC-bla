====================
Python API reference
====================

The objects below are within the Neosoft.cla module.

Vector
======


Matrix
======

.. class:: class Neosoft.cla.Matrix(height, width, values = 0)

    Initially, the matrix is empty (random values).

    .. code-block::

        >>> A = Matrix(m, n)
        >>> A(3, 4) = 5
        >>> print(A(3, 4))
        5
    
    Alternatively, you can also construct a Matrix and initialize it with the values of a python tuple or list using the list constructor.
    
    .. note::
        
        Python automatically saves the values in a RowMajor Matrix.

    .. code-block::

        >>> l = (6, 5, 3, -10, 3, 7, -3, 5, 12, 4, 4, 4, 0, 12, 0, -8)
        >>> A = Matrix(4, 4, l)

    The class also supports addition and multiplication with other matrices, vectors and scalars.

    .. code-block::

        A + B*v + 3*C + D*5

    Apart from that, Matrix is compatible with pickling and buffering.

    .. method:: Matrix.shape()

        returns the tuple (height, width)

    .. method:: Matrix.Data()

        returns the internal data pointer of the matrix


LapackLU
========

.. class:: class Neosoft.cla.LapackLU(Matrix)

    A LapackLU object can be constructed with the input of a Matrix object.

    .. code-block::
        
        A = Matrix(4, 4, (6, 5, 3, -10, 
                            3, 7, -3, 5, 
                            12, 4, 4, 4, 
                            0, 12, 0, -8))

        B = LapackLU(A)
    
    Using a LapackLU object, you are able to access every function of the C++ LapackLU class using the dot operator:
    - Inverse
    - Solve
    - LFactor
    - UFactor
    - PFactor

    .. code-block::

        B.Inverse()

        b = Vector(4)
        B.Solve(b)

    
