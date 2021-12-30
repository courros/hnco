import hnco
import hnco.function as fn

class MyFunction(fn.Function):
    def __init__(self, n):
        super().__init__()
        self.bv_size = n

    def evaluate(self, boolean_vars):
        return sum(boolean_vars)

    def get_bv_size(self):
        return self.bv_size

    def get_maximum(self):
        return self.bv_size

f = MyFunction(100)
