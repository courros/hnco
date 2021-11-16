import hnco
import hnco.algorithm as algo
import hnco.function as fn
import hnco.function.controller as ctrl
import hnco.function.modifier as mod
import hnco.map as map
import hnco.representation as rep
from hnco.random import Generator

# OneMax with 5 bits
f = fn.OneMax(5)
print(f)

# Random translation
tr = map.Translation()
tr.random(5)
print(tr)

# Compose function and translation
m = mod.FunctionMapComposition(f, tr)

# Stop on maximum
c = ctrl.StopOnMaximum(m)

# Random search with infinite number of iterations
a = algo.RandomSearch(5)
a.set_num_iterations(0)

# Maximise function until maximum is found
try:
    a.maximize([c])
except hnco.TargetReached:
    p = c.get_trigger()

# Print solution and its value
print(p)

# Print solution alone
solution = p.first
print(solution)

# Translated solution should give 11111
output = hnco.BitVector([0 for i in range(5)])
tr.map(solution, output)
print(output)

# Given seed (deterministic)
Generator.set_seed(10)
print(Generator.uniform())
print(Generator.uniform())

# Random seed
Generator.set_seed()
print(Generator.uniform())
print(Generator.uniform())

# hnco.HypercubeIterator is an iterable
for x in hnco.HypercubeIterator(5):
    print(x)
