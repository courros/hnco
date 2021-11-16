import math
import cmath

import hnco
import hnco.algorithm as algo
import hnco.function as fn
import hnco.function.controller as ctrl
import hnco.random
import hnco.representation as rep

hnco.random.Generator.set_seed()

class MyFunction(fn.UniversalFunction):
    def __init__(self):
        super().__init__()

    def evaluate(self, boolean_vars, integer_vars, real_vars, complex_vars, categorical_vars, permutation_vars):
        result = 0
        result += boolean_vars[0] + boolean_vars[1]
        result -= abs((integer_vars[0])**2 + (integer_vars[1])**2 - 2500)
        result -= (real_vars[0] - 0.5)**2 + (real_vars[1] - 0.5)**2
        result -= abs((complex_vars[0])**2 + complex_vars[0] + 1)
        result -= abs((complex_vars[1])**2 + 1)
        if categorical_vars[0] == 3:
            result += 1
        if categorical_vars[1] == 2:
            result += 1
        if permutation_vars[0][0] > permutation_vars[0][1]:
            result += 1
        if permutation_vars[0][0] > permutation_vars[0][1]:
            result += 1
        if permutation_vars[0][1] > permutation_vars[0][2]:
            result += 1
        if permutation_vars[1][0] < permutation_vars[1][1]:
            result += 1
        if permutation_vars[1][1] < permutation_vars[1][2]:
            result += 1
        if permutation_vars[1][2] < permutation_vars[1][3]:
            result += 1
        if permutation_vars[1][3] < permutation_vars[1][4]:
            result += 1
        return result

f0 = MyFunction()

integer_reps = []
integer_reps.append(rep.DyadicIntegerRepresentation(0, 255))
integer_reps.append(rep.DyadicIntegerRepresentation(0, 255))

real_rep = rep.DyadicFloatRepresentation(-2, 2, 8)

real_reps = []
real_reps.append(real_rep)
real_reps.append(real_rep)

complex_reps = []
complex_reps.append(rep.DyadicComplexRepresentation(real_rep, real_rep))
complex_reps.append(rep.DyadicComplexRepresentation(real_rep, real_rep))

categorical_reps = []
categorical_reps.append(rep.LinearCategoricalRepresentation(4))
categorical_reps.append(rep.LinearCategoricalRepresentation(3))

permutation_reps = []
permutation_reps.append(rep.PermutationRepresentation(3, 2))
permutation_reps.append(rep.PermutationRepresentation(5, 2))

f1 = fn.UniversalFunctionAdapter(f0, 2, integer_reps, real_reps, complex_reps, categorical_reps, permutation_reps)
print(f1)

f2 = ctrl.OnBudgetFunction(f1, 1000000)

a = algo.ParameterLessPopulationPyramid(f2.get_bv_size())

try:
    a.maximize([f2])
except hnco.LastEvaluation:
    a.finalize()
    hnco.bv_describe(a.get_solution().first, f2)
