=================
 Python binding
=================

There is a python binding for the library made with pybind11. The
whole library is available through the ``hnco`` module and its
submodules which are named after the C++ namespaces.

The source file of the binding is ``bindings/hnco.cc``.

Examples can be found under ``bindings/examples/``.

It is also possible to write a (fitness) function in python and use it
in the cli application hnco without recompilation.
