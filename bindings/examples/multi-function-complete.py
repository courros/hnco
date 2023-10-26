import hnco
import hnco.function as single
import hnco.multiobjective.function as multi

class MyFunction(multi.Function):

    def __init__(self, n):
        super().__init__()
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
