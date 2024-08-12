===============================================================
Optimizing a user-defined function from hnco (single objective)
===============================================================

.. _tutorial-hnco-parser:

Function parser
---------------

Suppose we want to minimize the function defined by

.. math::

  f(x, y, z) = x^2 + y^2 + z^2

over :math:`[-5, 5]^3`. In hnco, since the basic search space is a
hypercube (bit vectors), we have to specify variable representations,
each with its own type and bounds. For example::

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

No algorithm is specified in the command so that the default algorithm
(random local search) is applied with the default budget.

The last option ``--print-description`` prints the solution in terms
of representation, not in terms of bit vector.

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

180
  rep: bv -> double | parser: [double] -> double

181
  rep: bv -> long | parser: [long] -> long | cast to double

182
  rep: bv -> complex | parser: [complex] -> complex | z -> std::norm(z)

  Here, ``std::norm`` computes the squared magnitude of its argument.

183
  rep: bv -> int | cast to double | parser: [double] -> double

184
  rep: bv -> long, double, or set | parser: [double] -> double

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

.. literalinclude:: ../../bindings/examples/single-function.py
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
