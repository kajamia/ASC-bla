===============
Getting started
===============

Requirements
------------


- C++ compilers
- CMake
- Lapack
- Python
- pybind11

(and of course, it all needs to work well together)

..
    Python Installation
    -------------------

    To install, use:

    .. code-block::

        pip install git+https://github.com/kajamia/Neo-CLA.git@main


Installation
------------

Installation is via git-clone:

..  code-block:: bash
    
    git clone --recurse-submodules https://github.com/kajamia/Neo-CLA.git


To configure and build some tests do

..  code-block:: bash

    cd Neo-CLA
    mkdir build
    cd build
    cmake ..
    make

To compile for python:

.. code-block:: bash

    cd Neo-CLA
    pip install .


Usage: Python
-------------

ASCsoft.bla exposes the the classes Vector and Matrix:

.. code-block:: python

    from ASCsoft.bla import Matrix, Vector
    

Usage: C++
----------

To use Neo-CLA in your code, set the compiler include path properly, and include the header files

..  code-block:: cpp

    #include <vector.h>
    #include <matrix.h>

All objects are implemented in the namespace ASC_bla. To use them with less typing, you can set

..  code-block:: cpp
    
    namespace bla = ASC_bla;

or even

..  code-block:: cpp
    
    using namespace ASC_bla;

