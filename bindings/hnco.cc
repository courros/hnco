#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>

#include "hnco/functions/all.hh"

namespace py = pybind11;

using namespace hnco::function;
using namespace hnco;

PYBIND11_MAKE_OPAQUE(bit_vector_t);

class PyFunction : public Function {
public:
  using Function::Function;
  int get_bv_size()                             override { PYBIND11_OVERLOAD_PURE(int, Function, get_bv_size, ); }
  double evaluate(const bit_vector_t& x)        override { PYBIND11_OVERLOAD_PURE(double, Function, evaluate, x); }
};

PYBIND11_MODULE(hnco, m) {

  py::bind_vector<bit_vector_t>(m, "BitVector");

  py::class_<Function, PyFunction>(m, "Function")
    .def("get_bv_size", &Function::get_bv_size)
    .def("evaluate", &Function::evaluate);

  py::class_<OneMax>(m, "OneMax")
    .def(py::init<int>())
    .def("get_bv_size", &OneMax::get_bv_size)
    .def("evaluate", &OneMax::evaluate);

}
