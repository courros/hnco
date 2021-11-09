import hnco

from hnco import function as fn
from hnco import algorithm as algo
from hnco import map as map
from hnco.function import controller as ctrl
from hnco.function import modifier as mod
from hnco.random import Generator

f = fn.OneMax(5)
print(f)

tr = map.Translation()
tr.random(5)
print(tr)

m = mod.FunctionMapComposition(f, tr)

c = ctrl.StopOnMaximum(m)

a = algo.RandomSearch(5)
a.set_num_iterations(0)

try:
    a.maximize([c])
except hnco.TargetReached:
    p = c.get_trigger()

print(p)

solution = p.first
print(solution)

output = hnco.BitVector([0 for i in range(5)])

tr.map(solution, output)
print(output)

Generator.set_seed(10)
print(Generator.uniform())
print(Generator.uniform())

Generator.set_seed()
print(Generator.uniform())
print(Generator.uniform())

for x in hnco.HypercubeIterator(5):
    print(x)
