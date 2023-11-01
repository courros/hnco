===============
Acknowledgments
===============

Maxim Buzdalov has drawn my attention to the interest of incremental
evaluation.

Dirk Sudholt has pointed the discrepancy between theoretical and
experimental optimization times of (1+1) EA on OneMax, which has led
to the addition of the option ``--ea-allow-no-mutation`` to allow to
stay on the same bit vector when no mutation occurs.

The library embeds code from Brian W. Goldman's project
FastEfficientP3 to provide Hboa, Ltga, and P3 algorithms.

The library embeds code from Anton Bouter to provide the Gomea
algorithm.

The library is distributed with fparser. Quoting the preface of
fparser:

  This C++ library offers a class which can be used to parse and
  evaluate a mathematical function from a string (which might be eg.
  requested from the user). The syntax of the function string is
  similar to mathematical expressions written in C/C++ (the exact
  syntax is specified later in this document). The function can then
  be evaluated with different values of variables.

This project uses pybind11 for the python binding of the library and
for the communication with the python interpreter.
