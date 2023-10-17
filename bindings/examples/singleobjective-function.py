import hnco
import hnco.algorithm as algo
import hnco.function as fn
import hnco.random

hnco.random.Generator.set_seed()

class MyFunction(fn.Function):
    def __init__(self, n):
        fn.Function.__init__(self) # Mandatory
        self.bv_size = n

    def evaluate(self, bv):
        return sum(bv)

    def get_bv_size(self):
        return self.bv_size

f = MyFunction(100)

a = algo.OnePlusOneEa(f.get_bv_size())
a.set_num_iterations(1000)
a.maximize([f])
a.finalize()                    # Set the solution

print(a.get_solution())
