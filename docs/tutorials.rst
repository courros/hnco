=========
Tutorials
=========

-------------------------------------------------------------
Optimize a user-defined function from hnco (single objective)
-------------------------------------------------------------

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

.. _single-hnco-python:

Python
------

It is possible to define an arbitrary function in Python and maximize
it from hnco. For example, the file ``one-max.py`` in the directory
``bindings/examples/`` defines the custom function ``MyFunction``:

.. literalinclude:: ../bindings/examples/one-max.py
   :language: py

As can been seen, the method ``evaluate`` implements OneMax (bit
count). Observe that the variable ``f`` refers to an instance of
``MyFunction``. To maximize it from the command-line, enter the
command::

  hnco -F 1100 -p ./one-max.py --fn-name f

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

---------------------------------------------------------------
Optimize a user-defined function from Python (single objective)
---------------------------------------------------------------

Just as in Section :ref:`single-hnco-python`, the file
``singleobjective-function.py`` in the directory
``bindings/examples/`` defines the custom function ``MyFunction`` then
maximizes it with an algorithm, in this case ``OnePlusOneEa``:

.. code-block:: python

   import hnco
   import hnco.algorithm as algo
   import hnco.function as fn
   import hnco.random

   hnco.random.Generator.set_seed()

   class MyFunction(fn.Function):
       def __init__(self, n):
           fn.Function.__init__(self) # Mandatory
           self.bv_size = n

       def evaluate(self, bv):
           return sum(bv)

       def get_bv_size(self):
           return self.bv_size

   f = MyFunction(100)

   a = algo.OnePlusOneEa(f.get_bv_size())
   a.set_num_iterations(1000)
   a.maximize([f])
   a.finalize()                    # Set the solution

   print(a.get_solution())

To run the script, enter the command::

  python3 singleobjective-function.py

It is possible to go beyond bit vectors with the help of
representations. In hnco, a universal function is a function defined
on all the types represented in the library. For an example of
universal function, see the file
``singleobjective-universal-function.py`` in the directory
``bindings/examples/``
