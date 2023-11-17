.. ASC-bla documentation master file, created by
   sphinx-quickstart on Tue Aug 29 06:39:02 2023.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Hey! Welcome to Neo-CLA's documentation!
========================================

Neo-cla is a C++ library for basic linear algebra (bla) operations.
It is a fork of `ASC-bla <https://github.com/TUWien-ASC/ASC-bla>`_.
Thus, it provides its contents within the **ASC_bla** namespace.

Neo-CLA gives you:
 - Matrix and vector operations (fast C++)
 - A Python interface for simple yet performant code
 - An interface to Lapack for more performance
 - The custom high-performance submodule `Neo-CLA-HPC <https://github.com/kajamia/Neo-CLA-HPC>`_


You can create vectors and compute with vectors like:

..  code-block:: cpp
                 
   Vector<double> x(5), y(5), z(5);
   for (int i = 0; i < x.Size(); i++)
      x(i) = i;
   y = 5.0
   z = x+3*y;
   cout << "z = " << z << endl;


For matrices you can choose between row-major (`RowMajor`) or column-major (`ColMajor`) storage,
default is row-major.

..  code-block:: cpp

   Matrix<double,RowMajor> m1(5,3), m2(3,3);
   for (int i = 0; i < m1.Height(); i++)
     for (int j = 0; j < m1.Width(); j++)
       m1(i,j) = i+j;
   m2 = 3.7;
   Matrix product = m1 * m2;
   
You can extract a rows or a columns from a matrix:

..  code-block:: cpp

   Vector col1 = product.Col(1);



   
.. toctree::
   :maxdepth: 2
   :caption: Contents:
   
   Homepage <self>
   installation
   pyreference
   cppreference


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
