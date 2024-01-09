#######################################
SIMD - Single Instruction Multiple Data
#######################################

.. admonition:: about the code
    :class: tip

    The source code of the following objects can be found in simd_avx.h and simd.h,
    with examples and tests in demos/demo_simd.cc and demos/simd_timings.cc. The
    latter is there for performance tests.


The SIMD class
==============

SIMD<double, 2>
---------------

This class is the 2-element analogon to SIMD<double, 4>. It represents the __m128d type.

.. cpp:class:: template<> \
    SIMD<double, 2>

    .. cpp:function:: SIMD (double _val)

        There is a broadcasting constructor,

    .. cpp:function:: SIMD (double v0, double v1)

        one that gets every element individually,

    .. cpp:function:: SIMD (SIMD<double,1> v0, SIMD<double,1> v1)

        one that puts together smaller SIMDs,

    .. cpp:function:: SIMD (std::array<double,2> a)

        one that takes a vector

    .. cpp:function:: SIMD (double const * p)

        or a pointer array

    .. cpp:function:: SIMD (double const * p, SIMD<mask64,2> mask)

        or even a pointer array and a mask

    .. cpp:function:: static constexpr int Size()

        just returns 2

    .. cpp:function:: auto Val()

        the underlying __m128d

    .. cpp:function:: const double * Ptr() const

        the underlying __mm128d, as a double array

    .. cpp:function:: SIMD<double, 1> Lo() const

        first element

    .. cpp:function:: SIMD<double, 1> Hi() const

        second element

    .. cpp:function:: double operator[](size_t i) const

    .. cpp:function:: void Store (double * p) const
    .. cpp:function:: void Store (double * p, SIMD<mask64,2> mask) const

        store into p

    .. code-block:: C++

        SIMD<double, 2> f(2, 3);
        SIMD<double, 2> h(f, mask);

SIMD<mask64, 2>
---------------

The 2-element aequivalent to SIMD<mask64, 4>. It represents an __m128i.

.. cpp:class:: template<> \
    SIMD<mask64, 2>

    .. cpp:function:: SIMD (__m128i _mask)
    .. cpp:function:: SIMD (__m128d _mask)

        Both __m128i and the double variant are allowed.

    .. cpp:function:: auto Val() const

        The underlying __m128i.

    .. cpp:function:: mask64 operator[](size_t i) const

    .. cpp:function:: SIMD<mask64, 1> Lo() const
    .. cpp:function:: SIMD<mask64, 1> Hi() const

        first and second part of the SIMD

    .. code-block:: C++
        
        SIMD<mask64, 2> mask(_mm_cmp_pd(f.Val(),i.Val(), _CMP_GT_OQ));


SIMD operations
===============

Comparison operators
--------------------

Neo-CLA is able to do the following comparisons for SIMD objects (integers and doubles):

- <=
- <
- >=
- >

**Example for integers:**

.. cpp:function:: inline SIMD<mask64,4> operator>= (SIMD<int64_t,4> a, SIMD<int64_t,4> b)

Compares two SIMD int_64 objects and returns true/falls according to if a>=b. Analogue for the other comparison operators.
(Since there is no a>=b, we return !(b>a). For the same reason we return !(a>b) for a<=b).

.. code-block:: C++
    
    SIMD<int, 4> a(1,2,3,4);
    auto mask = (2 >= a);

    cout << "2 >= " << a << " = " << mask << endl;




**Example for doubles:**

.. cpp:function:: inline auto operator>= (SIMD<double,4> a, SIMD<double,4> b)

Compares two SIMD double objects and returns true/falls according to if a>=b. Analogue for the other comparison operators.

.. code-block:: C++
    
    SIMD<int, 4> a(1., 2, 3.1415);
    auto mask = (2. >= a);

    cout << "2 >= " << a << " = " << mask << endl;



Arithmetic operations
---------------------

Via operator overload, the following operations are defined for SIMD<double, 4>:

- simd+simd
- simd-simd
- simd*simd (component-wise)
- scalar*simd
- simd/simd (component-wise)
- scalar/simd (component-wise)

Additionally, the square root of a SIMD can be taken: :cpp:expr:`sqrt(b)`.

Examples:

.. code-block:: C++

    SIMD<double, 4> a(1, 3, 3.14, 42);
    SIMD<double, 4> b(2, 2, 0.5, 0.33);

    cout << a/b << endl;
    cout << 3/b << endl;
    cout << sqrt(b) << endl;

    SIMD<double, 4> c = 3 * a;
    SIMD<double, 4> d = a/b;

The output:

.. code-block::

    0.5, 1.5, 6.28, 127.273
    1.5, 1.5, 6, 9.09091
    1.41421, 1.41421, 0.707107, 0.574456