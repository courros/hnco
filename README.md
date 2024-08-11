# In Hypercubo Nigrae Capsulae Optimum (HNCO)

HNCO is a C++ library, a set of command-line tools, and a set of
scripts for the optimization of black box functions defined on bit
vectors. It aims at being flexible, fast, simple, and robust.

The project is published under the LGPL3 license.

The documentation is provided at
[hnco.readthedocs.io](https://hnco.readthedocs.io/en/latest/).

Library features:

- Bit vectors as basic search space
- Representations for integers, floats, complex numbers, categorical
  values, permutations, and finite sets of values
- Neighborhoods and neighborhood iterators for local search
- Maps from bit vectors to bit vectors
- Function decorators to control evaluation (budget...)
- Function decorators to modify functions (noise...)
- Support for incremental evaluation
- Maps and functions can be serialized to text files
- 29 builtin test functions
- Support for user defined functions (function parser, python
  function, plugin)
- Population with parallel evaluation
- 28 builtin algorithms (EAs...)
- Support for multiobjective optimization
- Python bindings
- 82 unit tests

Purposes:

- Provide a collection of standard functions
- Provide a collection of standard algorithms
- Favor reproducible research

Use cases:

- Solve practical problems
- Automate simulations
- Tune algorithms
- Compare algorithms
- Illustrate theoretical results
- Make conjectures
- Generate graphics and tables
