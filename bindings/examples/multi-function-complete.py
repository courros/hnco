import hnco
import hnco.function as single
import hnco.multiobjective.function as multi
import hnco.multiobjective.algorithm as algo
import hnco.random

hnco.random.Generator.set_seed()

class MyFunction(multi.Function):

    def __init__(self, n):
        multi.Function.__init__(self) # Mandatory
        self.f0 = single.OneMax(n)
        self.f1 = single.LeadingOnes(n)

    def evaluate(self, bv, value):
        value[0] = self.f0.evaluate(bv) # Minimize OneMax
        value[1] = -self.f1.evaluate(bv) # Maximize LeadingOnes

    def get_bv_size(self):
        return self.f0.get_bv_size()

    def get_output_size(self):
        return 2

f = MyFunction(10)

a = algo.Nsga2(f.get_bv_size(), f.get_output_size(), 100) # population_size = 100

a.set_num_iterations(200)
a.minimize([f])

solutions = a.get_solutions()
for bv in solutions.bvs:
    multi.bv_describe(bv, f)
