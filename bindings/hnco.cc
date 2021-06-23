/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

   This file is part of HNCO.

   HNCO is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   HNCO is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
   Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with HNCO. If not, see
   <http://www.gnu.org/licenses/>.

*/

#include <iostream>
#include <sstream>              // std::ostringstream

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/iostream.h>

#include "hnco/functions/all.hh"
#include "hnco/algorithms/all.hh"
#include "hnco/exception.hh"
#include "hnco/map.hh"

namespace py = pybind11;

using namespace hnco;

//
// Functions
//

class PyFunction: public function::Function {
public:
  using function::Function::Function;
  int get_bv_size()                             override { PYBIND11_OVERLOAD_PURE(int, function::Function, get_bv_size, ); }
  double evaluate(const bit_vector_t& x)        override { PYBIND11_OVERLOAD_PURE(double, function::Function, evaluate, x); }
  double get_maximum()                          override { PYBIND11_OVERLOAD(double, function::Function, get_maximum, ); }
  bool has_known_maximum()                      override { PYBIND11_OVERLOAD(bool, function::Function, has_known_maximum, ); }
  bool provides_incremental_evaluation()        override { PYBIND11_OVERLOAD(bool, function::Function, provides_incremental_evaluation, ); }
  void display()                                override { PYBIND11_OVERLOAD(void, function::Function, display, ); }
};

//
// Algorithms
//

class PyAlgorithm: public algorithm::Algorithm {
public:
  using algorithm::Algorithm::Algorithm;
  void maximize(const std::vector<function::Function *>& functions)
                                                override { PYBIND11_OVERLOAD_PURE(void, algorithm::Algorithm, maximize, functions); }
  void finalize()                               override { PYBIND11_OVERLOAD(void, algorithm::Algorithm, finalize, ); }
};

class PyIterativeAlgorithm: public algorithm::IterativeAlgorithm {
public:
  using algorithm::IterativeAlgorithm::IterativeAlgorithm;
  void iterate()                                override { PYBIND11_OVERLOAD_PURE(void, algorithm::IterativeAlgorithm, iterate, ); }
};

PYBIND11_MAKE_OPAQUE(bit_vector_t);

//
// Modules
//

PYBIND11_MODULE(hnco, module_hnco) {
 
  py::bind_vector<bit_vector_t>(module_hnco, "BitVector")
    .def("__str__",
         [](const bit_vector_t& bv) {
           std::ostringstream stream;
           bv_display(bv, stream);
           return stream.str();
         })
    ;

  //
  // Maps
  //

  py::class_<Map>(module_hnco, "Map")
    .def("map", &Map::map)
    .def("get_input_size", &Map::get_input_size)
    .def("get_output_size", &Map::get_output_size)
    .def("is_surjective", &Map::is_surjective)
    .def("display", static_cast<void (Map::*)()>(&Map::display))
    ;

  py::class_<Translation, Map>(module_hnco, "Translation")
    .def(py::init<>())
    .def("random", &Translation::random)
    .def("load", static_cast<void (Translation::*)(std::string)>(&Translation::load))
    .def("save", static_cast<void (Translation::*)(std::string) const>(&Translation::save))
    ;

  // Exceptions
  {
    using namespace hnco::exception;

    py::register_exception<MaximumReached>(module_hnco, "MaximumReached");

  }

  //
  // Functions
  //

  py::module module_hnco_function = module_hnco.def_submodule("function", "Functions");

  {
    using namespace hnco::function;

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

    py::class_<Decorator, Function>(module_hnco_function, "Decorator");

  }

  //
  // Controllers
  //

  py::module module_hnco_function_controller = module_hnco_function.def_submodule("controller", "Controllers");

  {
    using namespace function::controller;

    py::class_<Controller, function::Decorator>(module_hnco_function_controller, "Controller");

    py::class_<StopOnMaximum, Controller>(module_hnco_function_controller, "StopOnMaximum")
      .def(py::init<function::Function *>())
      .def("get_trigger", &StopOnMaximum::get_trigger)
      ;

  }

  //
  // Algorithms
  //

  py::module module_hnco_algorithm = module_hnco.def_submodule("algorithm", "Algorithms");

  {
    using namespace hnco::algorithm;

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
