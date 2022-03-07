import matplotlib.pyplot as plt
import statistics

import hnco
import hnco.algorithm as algo
import hnco.function as fn
import hnco.function.controller as ctrl

from hnco.random import Generator

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
