import hnco

import hnco.algorithm as algo
import hnco.function as fn
import hnco.function.controller as ctrl

from hnco.random import Generator

Generator.set_seed()

size = 100

f = fn.OneMax(size)
c1 = ctrl.ProgressTracker(f)
c1.set_log_improvement(True)
c2 = ctrl.StopOnMaximum(c1)

a = algo.OnePlusOneEa(size)

try:
    a.maximize([c2])
except hnco.TargetReached:
    pass
