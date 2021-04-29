#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/iostream.h>

#include "hnco/functions/all.hh"
#include "hnco/algorithms/all.hh"
#include "hnco/exception.hh"

namespace py = pybind11;

using namespace hnco;


class PyFunction : public function::Function {
public:
  using function::Function::Function;
  int get_bv_size()                             override { PYBIND11_OVERLOAD_PURE(int, function::Function, get_bv_size, ); }
  double evaluate(const bit_vector_t& x)        override { PYBIND11_OVERLOAD_PURE(double, function::Function, evaluate, x); }
  double get_maximum()                          override { PYBIND11_OVERLOAD(double, function::Function, get_maximum, ); }
  bool has_known_maximum()                      override { PYBIND11_OVERLOAD(bool, function::Function, has_known_maximum, ); }
  bool provides_incremental_evaluation()        override { PYBIND11_OVERLOAD(bool, function::Function, provides_incremental_evaluation, ); }
  void display()                                override { PYBIND11_OVERLOAD(void, function::Function, display, ); }
};


class PyAlgorithm : public algorithm::Algorithm {
public:
  using algorithm::Algorithm::Algorithm;
  void maximize(const std::vector<function::Function *>& functions)
                                                override { PYBIND11_OVERLOAD_PURE(void, algorithm::Algorithm, maximize, functions); }
  void finalize()                               override { PYBIND11_OVERLOAD(void, algorithm::Algorithm, finalize, ); }
};

class PyIterativeAlgorithm : public algorithm::IterativeAlgorithm {
public:
  using algorithm::IterativeAlgorithm::IterativeAlgorithm;
  void iterate()                                override { PYBIND11_OVERLOAD_PURE(void, algorithm::IterativeAlgorithm, iterate, ); }
};


PYBIND11_MODULE(hnco, module_hnco) {
 
  py::bind_vector<bit_vector_t>(module_hnco, "BitVector");

  // Functions
  {
    using namespace hnco::function;

    py::module module_hnco_function = module_hnco.def_submodule("function", "Functions");

    py::class_<Function, PyFunction>(module_hnco_function, "Function")
      .def(py::init<>())
      .def("display", static_cast<void (Function::*)()>(&Function::display)) // Since Function::display is overloaded
      .def("get_bv_size", &Function::get_bv_size)
      .def("get_maximum", &Function::get_maximum)
      .def("has_known_maximum", &Function::has_known_maximum)
      .def("provides_incremental_evaluation", &Function::provides_incremental_evaluation)
      .def("evaluate", &Function::evaluate)
      ;

    py::class_<OneMax, Function>(module_hnco_function, "OneMax")
      .def(py::init<int>())
      ;

    py::class_<LinearFunction, Function>(module_hnco_function, "LinearFunction")
      .def(py::init<>())
      .def("random", &LinearFunction::random)
      .def("load", &LinearFunction::load)
      .def("save", &LinearFunction::save)
      ;
  }

  // Algorithms
  {
    using namespace hnco::algorithm;

    py::module module_hnco_algorithm = module_hnco.def_submodule("algorithm", "Algorithms");

    py::class_<Algorithm, PyAlgorithm>(module_hnco_algorithm, "Algorithm")
      .def(py::init<int>())
      .def("get_bv_size", &Algorithm::get_bv_size)
      .def("get_solution", &Algorithm::get_solution)
      .def("maximize", &Algorithm::maximize)
      .def("finalize", &Algorithm::finalize)
      ;

    py::class_<IterativeAlgorithm, Algorithm, PyIterativeAlgorithm>(module_hnco_algorithm, "IterativeAlgorithm")
      .def(py::init<int>())
      .def("set_num_iterations", &IterativeAlgorithm::set_num_iterations)
      ;

    py::class_<RandomSearch, IterativeAlgorithm>(module_hnco_algorithm, "RandomSearch")
      .def(py::init<int>())
      ;
  }

}
