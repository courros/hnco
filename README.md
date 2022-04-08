# In Hypercubo Nigrae Capsulae Optimum (HNCO)

HNCO is a C++ library, a set of command-line tools, and a set of
scripts for the optimization of black box functions defined on bit
vectors. It aims at being flexible, fast, simple, and robust.

The documentation is provided at
[hnco.readthedocs.io](https://hnco.readthedocs.io/en/latest/).

Library features:

- 29 concrete functions
- 24 concrete algorithms
- 78 unit tests
- Maps from bit vectors to bit vectors
- Function decorators to control evaluation (budget...)
- Function decorators to modify functions (noise...)
- Maps and functions can be serialized to text files
- Neighborhoods and neighborhood iterators for local search
- Population with parallel evaluation
- Support for incremental evaluation
- Python bindings
- Representations for integer, float, complex, categorical and permutation values
- Support for multiobjective optimization

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
