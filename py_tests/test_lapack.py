from Neosoft.cla import Vector, Matrix, LapackLU


A = Matrix(4, 4, (6, 5, 3, -10, 
                    3, 7, -3, 5, 
                    12, 4, 4, 4, 
                    0, 12, 0, -8))


B = LapackLU(A)
b = Vector(4)

#print(B.Solve(b))
#print(b)
#print(B.Inverse())

print(B.LFactor())
print(B.UFactor())
print(B.PFactor())


