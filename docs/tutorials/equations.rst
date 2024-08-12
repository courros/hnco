===================
 Solving equations
===================

.. _tutorial-equation-hnco:

Solving single equations
------------------------

Suppose we want to solve the equation

.. math::

  z^2 + z + 1 = 0

where :math:`z` is a complex number whose real and imaginary parts
belong to :math:`[-5, 5]`. To recast the equation as a maximization
problem, we define the complex function

.. math::

  f(z) = z^2 + z + 1

and compose it with

.. math::

  z \mapsto -|z|^2

This is automatically done with the right parser so that we only have
to define :math:`f`. Just as in :ref:`tutorial-hnco-parser`, we have
to specify the representation of the complex variable :math:`z` which
is considered as two double variables named ``z_re`` (real part) and
``z_im`` (imaginary part). In the following command, we apply (1+1)-EA
to the resulting function with a budget of 1000 evaluations::

  hnco \
    -A 300 -b 1000 \
    -F 212 \
    --fp-expression "z^2 + z + 1" \
    --fp-representations "z_re: double(-5, 5); z_im: double(-5, 5)" \
    --fp-default-double-size 8 \
    --print-description

Here are the available parsers:

210
  rep: bv -> double | parser: [double] -> double | x -> -std::fabs(x)

211
  rep: bv -> long | parser: [long] -> long | n -> -std::fabs(n)

212
  rep: bv -> complex | parser: [complex] -> complex | z -> -std::norm(z)

  Here, ``std::norm`` computes the squared magnitude of its argument.

213
  rep: bv -> int | cast to double | parser: [double] -> double | x -> -std::fabs(x)

214
  rep: bv -> long, double, or set | parser: [double] -> double | x -> -std::fabs(x)

Solving systems of equations
----------------------------

Suppose we want to solve the equation

.. math::

  4x + 3y = 1

  3x + 2y = 2

where :math:`x` and :math:`y` both belong to :math:`[-10, 10]`.

To recast the equation as a minimization problem, we define two
functions

.. math::

  f(x) = 4x + 3y - 1

  g(x) = 3x + 2y - 2

and compose them with the absolute value. This is automatically done
with the right parser so that we only have to define :math:`f` and
:math:`g`. Just as in :ref:`tutorial-equation-hnco`, we have to
specify the representations of :math:`x` and :math:`y`. In the
following command, we apply the default algorithm (NSGA-II) for 10000
iterations with a population of 10 individuals to the resulting
function::

  hnco-mo \
    -i 10000 --ea-mu 10 \
    -F 210 \
    --fp-expression "4*x + 3*y - 1 :: 3*x + 2*y -2" \
    --fp-representations "x: double(-10, 10); y: double(-10, 10)" \
    --fp-default-double-size 8 \
    --print-description

Here are the available parsers:

210
  rep: bv -> double | parser: [double] -> [double] | x -> std::fabs(x)

211
  rep: bv -> long | parser: [long] -> [long] | n -> std::fabs(n)

212
  rep: bv -> complex | parser: [complex] -> [complex] | z -> std::norm(z)

  Here, ``std::norm`` computes the squared magnitude of its argument.

213
  rep: bv -> int | cast to double | parser: [double] -> [double] | x -> std::fabs(x)

214
  rep: bv -> long, double, or set | parser: [double] -> [double] | x -> std::fabs(x)
