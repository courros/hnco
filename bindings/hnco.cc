#include <pybind11/pybind11.h>

#include "hnco/functions/all.hh"

namespace py = pybind11;

using namespace hnco::function;

PYBIND11_MODULE(hnco, m) {
  py::class_<OneMax>(m, "OneMax")
    .def(py::init<int>())
    .def("get_bv_size", &OneMax::get_bv_size)
    .def("evaluate", &OneMax::evaluate);
}
