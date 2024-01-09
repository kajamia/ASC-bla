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

    To install just the Python part, use:

    .. code-block::

        pip install git+https://github.com/kajamia/Neo-CLA.git@main


Installation
------------

.. admonition:: Windows Support
    :class: warning

    Please note that this library currently only supports Linux and MacOS!


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

Neosoft.cla exposes the the classes Vector and Matrix:

.. code-block:: python

    from Neosoft.cla import Matrix, Vector
    

Usage: C++
----------

To use Neo-CLA in your code, set the compiler include path properly, and include the header files

..  code-block:: cpp

    #include <vector.h>
    #include <matrix.h>
    #include <lapack_interface.h>
    #include <fastmult.h>

All objects are implemented in the namespace Neo_CLA. To use them with less typing, you can set

..  code-block:: cpp
    
    namespace cla = Neo_CLA;

or even

..  code-block:: cpp
    
    using namespace Neo_CLA;

