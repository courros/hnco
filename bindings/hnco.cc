#include <iostream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/iostream.h>

#include "hnco/functions/all.hh"
#include "hnco/algorithms/all.hh"

namespace py = pybind11;

using namespace hnco::function;
using namespace hnco::algorithm;
using namespace hnco;


class PyFunction : public Function {
public:
  using Function::Function;
  int get_bv_size()                             override { PYBIND11_OVERLOAD_PURE(int, Function, get_bv_size, ); }
  double evaluate(const bit_vector_t& x)        override { PYBIND11_OVERLOAD_PURE(double, Function, evaluate, x); }
  double get_maximum()                          override { PYBIND11_OVERLOAD(double, Function, get_maximum, ); }
  bool has_known_maximum()                      override { PYBIND11_OVERLOAD(bool, Function, has_known_maximum, ); }
  bool provides_incremental_evaluation()        override { PYBIND11_OVERLOAD(bool, Function, provides_incremental_evaluation, ); }
};


class PyAlgorithm : public Algorithm {
public:
  using Algorithm::Algorithm;
  void maximize(const std::vector<function::Function *>& functions)
                                                override { PYBIND11_OVERLOAD_PURE(void, Algorithm, maximize, functions); }
  void finalize()                               override { PYBIND11_OVERLOAD(void, Algorithm, finalize, ); }
};


class PyIterativeAlgorithm : public IterativeAlgorithm {
public:
  using IterativeAlgorithm::IterativeAlgorithm;
  void iterate()                                override { PYBIND11_OVERLOAD_PURE(void, IterativeAlgorithm, iterate, ); }
};

PYBIND11_MODULE(hnco, m) {
 
  py::bind_vector<bit_vector_t>(m, "BitVector");

  py::class_<Function, PyFunction>(m, "Function")
    .def(py::init<>())
    .def("get_bv_size", &Function::get_bv_size)
    .def("get_maximum", &Function::get_maximum)
    .def("has_known_maximum", &Function::has_known_maximum)
    .def("provides_incremental_evaluation", &Function::provides_incremental_evaluation)
    .def("evaluate", &Function::evaluate);

  py::class_<OneMax, Function>(m, "OneMax")
    .def(py::init<int>());

  py::class_<Algorithm, PyAlgorithm>(m, "Algorithm")
    .def(py::init<int>())
    .def("get_bv_size", &Algorithm::get_bv_size)
    .def("get_solution", &Algorithm::get_solution)
    .def("maximize", &Algorithm::maximize)
    .def("finalize", &Algorithm::finalize);

  py::class_<IterativeAlgorithm, Algorithm, PyIterativeAlgorithm>(m, "IterativeAlgorithm")
    .def(py::init<int>())
    .def("set_num_iterations", &IterativeAlgorithm::set_num_iterations);

  py::class_<RandomSearch, IterativeAlgorithm>(m, "RandomSearch")
    .def(py::init<int>());

}
