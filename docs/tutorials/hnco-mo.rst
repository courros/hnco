=========
Tutorials
=========

---------------------------------------------------------------
Optimizing a user-defined function from hnco (single objective)
---------------------------------------------------------------

Function parser
---------------

Suppose we want to minimize the function defined by::

  f(x, y, z) = x^2 + y^2 + z^2

over [-5, 5]^3. In hnco, since the basic search space is a hypercube
(bit vectors), we have to specify variable representations, each with
its own type and bounds. For example::

  hnco \
    -F 180 \
    --fp-expression "x^2 + y^2 + z^2" \
    --fp-representations "x: double(-5, 5); y: double(-5, 5); z: double(-5, 5)" \
    --fp-default-double-size 8 \
    --minimize \
    --print-description

Each representation occupies 8 bits of the bit vector. The size or
precision can be individually specified as in ``double(-5, 5, size =
4)`` or ``double(-5, 5, precision = 1e-3)``. If no representation is
given for some variable, the default double representation is used
instead.

Minimization, instead of maximization, is achieved with the option
``--minimize``.

The last option ``--print-description`` prints the solution in terms
of representation, not in terms of bit vector.

The function is minimized with the default algorithm (random local
search) but all algorithms can be used instead.

Both the function and representations can be specified in files
instead of the command-line::

  hnco \
    -F 180 \
    --path ./function.txt \
    --fp-representations-path ./representations.txt \
    --fp-default-double-size 8 \
    --minimize \
    --print-description

where ``function.txt`` contains::

  x^2 + y^2 + z^2

and ``representations.txt`` contains::

  x: double(-5, 5);
  y: double(-5, 5);
  z: double(-5, 5)

See ``hnco --help-fp`` for more options for the function parser.

The function can be displayed with the option ``--fn-display``::

  Warning: DecoratedFunctionFactory::make_function: After _function_factory.make(), bv_size changed from 100 to 24
  ParsedMultivariateFunction:
  Variables: x, y, z
  Expression:
  x^2+y^2+z^2
  Representations:
  DyadicFloatRepresentation [-5, 5) (8 bits)
  DyadicFloatRepresentation [-5, 5) (8 bits)
  DyadicFloatRepresentation [-5, 5) (8 bits)

Here are the available parsers:

- 180 with rep: bv -> double and parser: [double] -> double
- 181 with rep: bv -> long and parser: [long] -> long, cast to double
- 182 with rep: bv -> complex and parser: [complex] -> complex, square of the magnitude
- 183 with rep: bv -> int, cast to double, and parser: [double] -> double
- 184 with rep: bv -> long, double, or set, and parser: [double] -> double

For example, the last parser allows to minimize the same function as
above with the following representations::

  x: double(-5, 5);
  y: long(-5, 5);
  z: set(1.1, 2.2, -3.3)

In this case, here is the displayed function::

  Warning: DecoratedFunctionFactory::make_function: After _function_factory.make(), bv_size changed from 100 to 14
  ParsedMultivariateFunction:
  Variables: x, y, z
  Expression:
  x^2+y^2+z^2
  Representations:
  DyadicFloatRepresentation [-5, 5) (8 bits)
  DyadicIntegerRepresentation [-5..5] (4 bits)
  ValueSetRepresentation {-3.3, 1.1, 2.2} (2 bits)

Python
------

It is possible to define an arbitrary function in Python and maximize
it from hnco. For example, the file ``single-function.py`` in the
directory ``bindings/examples/`` defines the custom function
``MyFunction``:

.. literalinclude:: ../bindings/examples/single-function.py
   :language: py

As can been seen, the method ``evaluate`` implements OneMax (bit
count). Observe that the variable ``f`` refers to an instance of
``MyFunction``. To maximize it from the command-line, enter the
command::

  hnco -F 1100 --path ./single-function.py --fn-name f

Plugin
------

It is possible to load a dynamic library and maximize a given function
in this library with hnco. For example, the file ``examples/onemax.c``
contains the following definition:

.. code-block:: c

   double onemax(const unsigned char *data, size_t len)
   {
     int result = 0;
     size_t i;
     for (i = 0; i < len; i++)
       if (data[i])
         result++;
     return result;
   }

In the build directory, enter the following commands::

  cd examples
  make plugin

which is equivalent to::

  gcc -fPIC -c $(top_srcdir)/examples/onemax.c
  gcc -shared -Wl,-soname,libfoo.so -o libfoo.so onemax.o

Then, you can maximize ``onemax`` with::

  hnco -F 1000 --path ./libfoo.so --fn-name onemax

-----------------------------------------------------------------
Optimizing a user-defined function from Python (single objective)
-----------------------------------------------------------------

It is possible to define and optimize an arbitrary function in Python.
The file ``single-function.py`` in the directory
``bindings/examples/`` defines and maximizes the custom function
``MyFunction``:

.. literalinclude:: ../bindings/examples/single-function-complete.py
   :language: py

To run the script, enter the command::

  python3 single-function.py

--------------------------------------------------------------------------
Optimizing a user-defined function from hnco-mo (multiobjective objective)
--------------------------------------------------------------------------

Function parser
---------------

Suppose we want to optimize the mixed-integer function defined by::

  f(x, y) = (sin(x) + cos(y), x^2 + y^2)

over [-10, 10] x [-50 .. 50]. The command-line interface of
``hnco-mo`` is similar to that of ``hnco``. Therefore, we will only
outline the differences between them. The different objectives are
separated by double colon::

  hnco-mo \
    -F 184 \
    --fp-expression "sin(x) + cos(y) :: x^2 + y^2" \
    --fp-representations "x: double(-10, 10, precision = 0.001); y: long(-50, 50)" \
    --print-description

The last option ``--print-description`` prints the non dominated
solutions in terms of representation, not in terms of bit vector. The
option ``--print-pareto-front`` only prints the objectives of non
dominated solutions. This is useful to visualize the Pareto front, for
example with gnuplot.

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

Python
------

It is possible to define an arbitrary function in Python and optimize
it from ``hnco-mo``. For example, the file
``multi-function.py`` in the directory ``bindings/examples/``
defines the custom function ``MyFunction``:

.. literalinclude:: ../bindings/examples/multi-function.py
   :language: py

Observe that the variable ``f`` refers to an instance of
``MyFunction``. To optimize it from the command-line, enter the
command::

  hnco-mo -F 1100 --path ./multi-function.py --fn-name "f" --print-description

Since no representation is need in this example, the option
``--print-description`` simply prints the binary description of
solutions along with their objectives.

-------------------------------------------------------------------------
Optimizing a user-defined function from Python (multiobjective objective)
-------------------------------------------------------------------------

It is possible to define and optimize an arbitrary function in Python.
For example, the file ``multi-function-complete.py`` in the directory
``bindings/examples/`` defines and optimizes the custom function
``MyFunction``:

.. literalinclude:: ../bindings/examples/multi-function-complete.py
   :language: py

Observe that the variable ``f`` refers to an instance of
``MyFunction``. To run the script, enter the command::

  python3 multi-function-complete.py
