# import from the installed ASCsoft package:
from ASCsoft.bla import Vector, Matrix

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