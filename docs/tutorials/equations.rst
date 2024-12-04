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
    -F 512 \
    --fp-expression "z^2 + z + 1" \
    --fp-representations "z_re: double(-5, 5); z_im: double(-5, 5)" \
    --fp-default-double-size 8 \
    --print-description

Here is a possible output::

  Warning: DecoratedFunctionFactory::make_function: After _function_factory.make(), bv_size changed from 100 to 16
  Warning: CommandLineApplication::maximize: Last evaluation
  z = (-0.507812,-0.859375)

whereas the exact solution is

.. math::

   \frac{-1\pm i\sqrt{3}}{2} \approx -0.5 \pm 0.866 \,.

By design, the algorithm produces a single solution which may or may
not be a good approximation to an exact solution. There is no
guarantee on the precision. Depending on the case, even if the
equation has no solution, the algorithm will still give an answer!
Sometimes, the algorithm gets stuck in a local maximum so that it is
useful to monitor progress with the flag ``--log-improvement`` and
rerun the algorithm several times.

Here are the available parsers:

510
  ``rep: bv -> double | parser: [double] -> double | x -> -std::fabs(x)``

511
  ``rep: bv -> long | parser: [long] -> long | n -> -std::fabs(n)``

512
  ``rep: bv -> complex | parser: [complex] -> complex | z -> -std::norm(z)``

  Here, ``std::norm`` computes the squared magnitude of its argument.

513
  ``rep: bv -> int | cast to double | parser: [double] -> double | x -> -std::fabs(x)``

514
  ``rep: bv -> long, double, or set | parser: [double] -> double | x -> -std::fabs(x)``

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
following command, we apply the default algorithm (NSGA-II) for 1000
iterations with a population of 100 individuals to the resulting
function::

  hnco-mo \
    -i 1000 --ea-mu 100 \
    -F 530 \
    --fp-expression "4*x + 3*y - 1 :: 3*x + 2*y - 2" \
    --fp-representations "x: double(-10, 10); y: double(-10, 10)" \
    --fp-default-double-size 16 \
    --print-description

Here is a possible output::

  Objectives: (1.16825e-05, 1.49012e-08)
  x = 3.99353
  y = -4.99023

  Objectives: (1.16825e-05, 1.49012e-08)
  x = 3.99353
  y = -4.99023

  Objectives: (3.72529e-09, 6.88806e-06)
  x = 3.992
  y = -4.98932

  Objectives: (3.72529e-09, 6.88806e-06)
  x = 3.992
  y = -4.98932

  Objectives: (9.68948e-06, 3.35276e-08)
  x = 3.99323
  y = -4.98993

  Objectives: (4.82798e-06, 6.29574e-07)
  x = 3.99323
  y = -4.99023

  Objectives: (4.5076e-07, 4.05684e-06)
  x = 3.99261
  y = -4.98993

  Objectives: (2.5183e-06, 1.97068e-06)
  x = 3.99261
  y = -4.98962

  Objectives: (9.53674e-07, 2.92063e-06)
  x = 3.99292
  y = -4.99023

  Objectives: (3.58e-06, 1.20699e-06)
  x = 3.99292
  y = -4.98993

whereas the unique exact solution is :math:`(4, -5)`.

It should be noted that, sometimes, the algorithm puts too much
emphasis on a single objective, which can be detrimental to the other
objectives. Hence, the set of non-dominated solutions can be void of
any useful solution. In this case, it is still possible to combine the
objectives into a single function and apply single objective
optimization to it with ``hnco`` as seen in
:ref:`tutorial-equation-hnco`.

Here are the available parsers:

530
  ``rep: bv -> double | parser: [double] -> [double] | x -> std::fabs(x)``

531
  ``rep: bv -> long | parser: [long] -> [long] | n -> std::fabs(n)``

532
  ``rep: bv -> complex | parser: [complex] -> [complex] | z -> std::norm(z)``

  Here, ``std::norm`` computes the squared magnitude of its argument.

533
  ``rep: bv -> int | cast to double | parser: [double] -> [double] | x -> std::fabs(x)``

534
  ``rep: bv -> long, double, or set | parser: [double] -> [double] | x -> std::fabs(x)``
