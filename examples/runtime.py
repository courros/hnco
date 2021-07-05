import hnco

from hnco import function as fn
from hnco import algorithm as algo
from hnco.function import controller as ctrl
from hnco.random import Generator

import matplotlib.pyplot as plt
import statistics

Generator.set_seed()

sizes = list(range(10, 210, 10))
runtimes = []

for size in sizes:
    f = fn.OneMax(size)
    cc = ctrl.CallCounter(f)
    c = ctrl.StopOnMaximum(cc)
    a = algo.OnePlusOneEa(size)
    results = []
    for run in range(10):
        try:
            a.maximize([c])
        except hnco.TargetReached:
            results.append(cc.get_num_calls())
    runtimes.append(statistics.mean(results))

plt.plot(sizes, runtimes)
plt.show()
