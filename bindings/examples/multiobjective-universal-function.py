import math
import cmath

import hnco
import hnco.multiobjective.algorithm as algo
import hnco.multiobjective.function as multi
import hnco.random
import hnco.representation as rep

hnco.random.Generator.set_seed()

class MyFunction(multi.UniversalFunction):

    def __init__(self):
        super().__init__()

    def get_output_size(self):
        return 2

    def evaluate(self, boolean_vars, integer_vars, float_vars, complex_vars, categorical_vars, permutation_vars, value):
        # First objective
        result0 = 0
        result0 -= boolean_vars[0] + boolean_vars[1]
        result0 += (float_vars[0] - 0.5)**2 + (float_vars[1] - 0.5)**2
        result0 += abs((complex_vars[1])**2 + 1)
        if permutation_vars[0][0] > permutation_vars[0][1]:
            result0 -= 1
        if permutation_vars[0][0] > permutation_vars[0][1]:
            result0 -= 1
        if permutation_vars[0][1] > permutation_vars[0][2]:
            result0 -= 1
        # Second objective
        result1 = 0
        result1 += abs((integer_vars[0])**2 + (integer_vars[1])**2 - 2500)
        result1 += abs((complex_vars[0])**2 + complex_vars[0] + 1)
        if categorical_vars[0] == 3:
            result1 -= 1
        if categorical_vars[1] == 2:
            result1 -= 1
        if permutation_vars[1][0] < permutation_vars[1][1]:
            result1 -= 1
        if permutation_vars[1][1] < permutation_vars[1][2]:
            result1 -= 1
        if permutation_vars[1][2] < permutation_vars[1][3]:
            result1 -= 1
        if permutation_vars[1][3] < permutation_vars[1][4]:
            result1 -= 1
        # Output
        value[0] = result0
        value[1] = result1

uf = MyFunction()

integer_reps = []
integer_reps.append(rep.DyadicIntegerRepresentation(0, 255))
integer_reps.append(rep.DyadicIntegerRepresentation(0, 255))

float_rep = rep.DyadicFloatRepresentation(-2, 2, 8)

float_reps = []
float_reps.append(float_rep)
float_reps.append(float_rep)

complex_reps = []
complex_reps.append(rep.DyadicComplexRepresentation(float_rep, float_rep))
complex_reps.append(rep.DyadicComplexRepresentation(float_rep, float_rep))

categorical_reps = []
categorical_reps.append(rep.LinearCategoricalRepresentation(4))
categorical_reps.append(rep.LinearCategoricalRepresentation(3))

permutation_reps = []
permutation_reps.append(rep.PermutationRepresentation(3, 2))
permutation_reps.append(rep.PermutationRepresentation(5, 2))

ufa = multi.UniversalFunctionAdapter(uf, 2, integer_reps, float_reps, complex_reps, categorical_reps, permutation_reps)
print(ufa)

a = algo.Nsga2(ufa.get_bv_size(), ufa.get_output_size(), 100)

a.set_num_iterations(200)
a.minimize([ufa])
a.finalize()
