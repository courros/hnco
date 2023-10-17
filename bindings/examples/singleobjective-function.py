import hnco
import hnco.function as fn

class MyFunction(fn.Function):
    def __init__(self, n):
        fn.Function.__init__(self) # Mandatory
        self.bv_size = n

    def evaluate(self, bv):
        return sum(bv)

    def get_bv_size(self):
        return self.bv_size

    def has_known_maximum(self):
        return True

    def get_maximum(self):
        return self.bv_size

f = MyFunction(100)
