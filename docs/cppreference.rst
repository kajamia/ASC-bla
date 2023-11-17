=================
C++ API reference
=================

.. matrix sachen dokumentieren
.. youtube/google: ("restructured text" und "sphinx")
.. das dokumentieren was aufgabe war
.. norbert will JEDEN code in blöcken haben


Matrix
======

This is the main entry point to Neo-CLA.

.. cpp:class:: Matrix

    .. cpp:classmethod:: Matrix (size_t height, size_t width, std::initializer_list<T> list)
    
    The initializer list constructor stores an initalizer list into a matrix.

    .. code-block:: cpp

        bla::Matrix<double> E(4, 7, {1, 2, 3, 4, 5, 6, 7,
                                    8, 9, 10, 11, 12, 13, 14,
                                    15, 16, 17, 18, 19, 20, 21,
                                    22, 23, 24, 25, 26, 27, 28});
                                
    
