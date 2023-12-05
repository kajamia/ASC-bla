# import from the installed ASCsoft package:
from Neosoft.cla import Vector, Matrix

import time
import pickle
import numpy as np


A = Matrix(2, 2)
A[0, 0] = 5
A[1, 1] = 3
A[1, 0] = 0
A[0, 1] = 0

B = Matrix(2, 2)
B[1, 0] = 7
B[0, 1] = 11
B[0, 0] = 0
B[1, 1] = 0

print(A+B)
print(3*A)

a = Vector(2)
a[0] = 1
a[1] = 3
b = np.asarray(a)
G = np.asarray(A)
print(G, b, G@b)

n = 1000
for n in [10, 100, 1000]:
    print(f"n = {n}")
    C = Matrix(n, n)
    D = Matrix(n, n)

    start = time.perf_counter()
    C.timed_mult(D)
    stop = time.perf_counter()
    print(F"total time for multiplication: {stop-start} seconds")

    # now for numpy:
    E = np.asarray(C, order="C")
    F = np.asarray(D, order="C")

    start = time.perf_counter()
    E@F
    stop = time.perf_counter()
    print(F"numpy took {stop-start} seconds")


# pickling:

gherkin = pickle.dumps(A)
print(pickle.loads(gherkin))