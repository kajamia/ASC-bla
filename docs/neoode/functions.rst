====================
The function algebra
====================

This family of classes is a versatile utility for implementing all kinds of functions.
All of them derive from NonlinearFunction and enable the user to write rather elegant code:

.. code-block:: C++

    auto f = 3*make_shared<IdentityFunction>(3) + make_shared<ConstantFunction>(3.1415)

NonlinearFunction
=================

ConstantFunction
================

…

BlockFunction
=============

.. cpp:class:: BlockFunction : public NonlinearFunction

    .. cpp:function:: BlockFunction(shared_ptr<NonlinearFunction> component, size_t size)
    
    BlockFunction first evaluates the function component(x) and then stores it in df,
    but size times after each other. EvaluateDeriv behaves accordingly.