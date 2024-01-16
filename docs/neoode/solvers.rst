=======
Solvers
=======

Basic solvers
-------------

The following solvers are available:

- Explicit Euler (EE)
- Implicit Euler (IE)
- Crank-Nicholson (CN)

.. cpp:function:: void SolveODE_EE(double tend, int steps, VectorView<double> y, shared_ptr<NonlinearFunction> rhs, std::function<void(double, VectorView<double>)> callback = nullptr)
.. cpp:function:: void SolveODE_IE(double tend, int steps, VectorView<double> y, shared_ptr<NonlinearFunction> rhs, std::function<void(double, VectorView<double>)> callback = nullptr)
.. cpp:function:: void SolveODE_CN(double tend, int steps, VectorView<double> y, shared_ptr<NonlinearFunction> rhs, std::function<void(double, VectorView<double>)> callback = nullptr)

    This solves an ODE using the explicit euler method. The ODE has to be in autonomous form.

    :param tend: The domain of the solution function will be [0, tend].
    :param steps: The amount of aequidistant points within [0, tend] for which a solution will be computed.
    :param y: Start values, the last value of y is the time, the latter should be 0.
    :param rhs: Right-hand-side of the ODE, a function in Neo-ODE's function algebra.
    :param callback: An optional function to log values. It takes the position within [0, tend] and the computed value.

    .. code-block::

        double tend = 4*M_PI;
        int steps = 100;
        Vector<> y { 1, 0 };
        auto rhs = make_shared<MassSpring>();
        
        SolveODE_IE(tend, steps, y, rhs,
                    [](double t, VectorView<double> y) { cout << "IE " << t << " \t " << y(0) << " \t " << y(1) << endl; });

..
    Mechanical systems
    ------------------

    Apart from that, there are also solvers for mechanical systems:

    - Generalized alpha
    - Newmark

    .. cpp:function:: void SolveODE_Alpha (double tend, int steps, double rhoinf, VectorView<double> x, VectorView<double> dx, VectorView<double> ddx, shared_ptr<NonlinearFunction> rhs, shared_ptr<NonlinearFunction> mass, std::function<void(double,VectorView<double>)> callback = nullptr)

        tend, steps and callback are as above.
        This solves 