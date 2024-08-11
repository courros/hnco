================================================
 Optimizing a user-defined function from Python
================================================

Single objective
----------------

It is possible to define and optimize an arbitrary function in Python.
The file ``single-function.py`` in the directory
``bindings/examples/`` defines and maximizes the custom function
``MyFunction``:

.. literalinclude:: ../../bindings/examples/single-function-complete.py
   :language: py

To run the script, enter the command::

  python3 single-function.py

Multiobjective
--------------

It is possible to define and optimize an arbitrary function in Python.
For example, the file ``multi-function-complete.py`` in the directory
``bindings/examples/`` defines and optimizes the custom function
``MyFunction``:

.. literalinclude:: ../../bindings/examples/multi-function-complete.py
   :language: py

Observe that the variable ``f`` refers to an instance of
``MyFunction``. To run the script, enter the command::

  python3 multi-function-complete.py
