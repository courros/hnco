=================
 Python binding
=================

There is a python binding for the library made with pybind11. Most of
the library is available through the ``hnco`` module and its
submodules which are named after the hnco C++ namespaces.

It is also possible to define a (fitness) function in python and
maximize it from hnco without recompilation.

The source file of the binding is ``bindings/hnco.cc``.

Examples can be found under ``bindings/examples/``.
