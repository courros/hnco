from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

__version__ = "0.21"

# The main interface is through Pybind11Extension.
# * You can add cxx_std=11/14/17, and then build_ext can be removed.
# * You can set include_pybind11=false to add the include directory yourself,
#   say from a submodule.
#
# Note:
#   Sort input source files if you glob sources to ensure bit-for-bit
#   reproducible builds (https://github.com/pybind/python_example/pull/53)

ext_modules = [
    Pybind11Extension(
        "hnco",
        ["hnco.cc"],
        libraries = ['hnco', 'boost_serialization', 'dl', 'gmp'],
        # Example: passing in the version to the compiled code
        define_macros = [('VERSION_INFO', __version__)],
        ),
]

setup(
    name="hnco",
    version=__version__,
    author="Arnaud Berny",
    author_email="dev@courros.fr",
    url="https://github.com/courros/hnco",
    description="Python module for hnco",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext}
)
