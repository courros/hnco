==================================================================
 Optimizing a user-defined function from hnco-mo (multiobjective)
==================================================================

Function parser
---------------

Suppose we want to minimize the mixed-integer function defined by

.. math::

  f(x, y) = (\sin(x) + \cos(y), x^2 + y^2)

over :math:`[-10, 10] \times [-50 .. 50]`. The command-line interface
of ``hnco-mo`` is similar to that of ``hnco``. We have to specify the
variable representations, each with its own type (in this case a
``double`` and a ``long``) and bounds. The different objectives are
separated by double colon. In the following command, we apply the
default algorithm (NSGA-II) for 100 iterations with a population of 3
individuals to the resulting function::

  hnco-mo \
    -i 100 --ea-mu 3 \
    -F 184 \
    --fp-expression "sin(x) + cos(y) :: x^2 + y^2" \
    --fp-representations "x: double(-10, 10, precision = 0.001); y: long(-50, 50)" \
    --print-description

The last option ``--print-description`` prints the non dominated
solutions in terms of representation, not in terms of bit vector. The
option ``--print-pareto-front`` only prints the objectives of non
dominated solutions. This is useful to visualize the Pareto front, for
example with gnuplot.

Here is a possible output::

  Warning: CommandLineApplication::make_function: After _function_factory.make(), bv_size changed from 100 to 22
  Objectives: (1.00061, 3.72529e-07)
  x = 0.000610352
  y = 0

  Objectives: (-1.94874, 34.0061)
  x = 5.00061
  y = 3

  Objectives: (-1.94223, 12.5386)
  x = -1.8811
  y = 3

Both the function and representations can be specified in files
instead of the command-line::

  hnco-mo \
    -F 184 \
    --path ./function.txt \
    --fp-representations-path ./representations.txt \
    --print-description

where ``function.txt`` contains::

  A := sin(x) + cos(y); A

  ::

  B := x^2 + y^2; B

and ``representations.txt`` contains::

  x: double(-10, 10, precision = 0.001);
  y: long(-50, 50)

The function parser allows the definition of variables (in this case A
and B) which increases the readibility in the case of complex
functions. It should be noted that each definition is local to its
objective.

See ``hnco-mo --help-fp`` for more options for the function parser.

The function can be displayed with the option ``--fn-display``::

  Warning: CommandLineApplication::make_function: After _function_factory.make(), bv_size changed from 100 to 22
  ParsedMultivariateFunction:
  Variables: x, y
  Objectives:
  0: x, y -> sin(x)+cos(y)
  1: x, y -> x^2+y^2
  Representations:
  DyadicFloatRepresentation [-10, 10) (15 bits)
  DyadicIntegerRepresentation [-50..50] (7 bits)

Here are the available parsers:

180
  ``rep: bv -> double | parser: [double] -> [double]``

181
  ``rep: bv -> long | parser: [long] -> [long] | cast to double``

182
  ``rep: bv -> complex | parser: [complex] -> [complex] | z -> std::norm(z)``

  Here, ``std::norm`` computes the squared magnitude of its argument.

183
  ``rep: bv -> int | cast to double | parser: [double] -> [double]``

184
  ``rep: bv -> long, double, or set | parser: [double] -> [double]``

Python
------

It is possible to define an arbitrary function in Python and optimize
it from ``hnco-mo``. For example, the file
``multi-function.py`` in the directory ``bindings/examples/``
defines the custom function ``MyFunction``:

.. literalinclude:: ../../bindings/examples/multi-function.py
   :language: py

Observe that the variable ``f`` refers to an instance of
``MyFunction``. To optimize it from the command-line, enter the
command::

  hnco-mo -F 1100 --path ./multi-function.py --fn-name "f" --print-description

Since no representation is need in this example, the option
``--print-description`` simply prints the binary description of
solutions along with their objectives.
