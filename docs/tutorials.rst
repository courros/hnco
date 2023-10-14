=========
Tutorials
=========

------------------------------------------
Optimize a user-defined function from hnco
------------------------------------------

Parser
------

Suppose we want to minimize the function defined by::

  f(x, y, z) = x^2 + y^2 + z^2

over [-1, 1]^3. Each variable can have its own domain. In hnco, since
the basic search space is the hypercube (bit vectors), we have to
specify representations with type, bounds, and precision. We would
enter::

  hnco -F 180 --fp-expression "-(x^2+y^2+z^2)" \
              --fp-representations "x: double(-1, 1, precision = 1e-3); y: double(-1, 1, size = 8); z: double(-1, 1)" \
              --fp-default-double-size 4 \
              --print-description

The precision can be implicitly defined in terms of size, that is the
number of bits used for the representation. If no precision is given,
then a default precision must be provided. If no representation is
given for some variable, the default representation is used instead.
The resulting function can be displayed with the option
``--fn-display``. The last option ``--print-description`` prints the
solution in terms of representation, not in terms of bit vector.

Both the function and representations can be specified in files
instead of the command-line::

  hnco -F 180 --fp-expression-source 1 \
              --path ./function.txt \
              --fp-representations-source 1 \
              --fp-representations-path ./representations.txt \
              --fp-default-double-size 4 \
              --print-description

where ``function.txt`` contains::

  -(x^2+y^2+z^2)

and ``representations.txt`` contains::

  x: double(-1, 1, precision = 1e-3);
  y: double(-1, 1, size = 8);
  z: double(-1, 1)

See ``hnco --help-fp`` for more options for the parser.

Python
------

It is possible to define an arbitrary function in Python and maximize
it from hnco without recompilation. For example, suppose that the file
``one-max.py`` contains the following Python code:

.. literalinclude:: ../bindings/examples/one-max.py
   :language: py

As can been seen, the class `MyFunction` provides an implementation of
OneMax. Observe that the variable `f` refers to an instance of
`MyFunction`. To maximize it from the command-line, enter the
command::

  hnco -F 1100 -p ./one-max.py --fn-name f

Plugin
------

tbd.

--------------------------------------------
Optimize a user-defined function from Python
--------------------------------------------

tbd.
