==============
 Introduction
==============

HNCO is a C++ library, a set of command-line tools, and a set of
scripts for the optimization of black box functions defined on bit
vectors. It aims at being flexible, fast, simple, and robust.

The project is published under the LGPL3 license.

The source code is available on `github
<https://github.com/courros/hnco>`_.

Library features:

- Bit vectors as basic search space
- Representations for integer, float, complex, categorical and
  permutation values as well as finite sets of values
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
- 80 unit tests

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
