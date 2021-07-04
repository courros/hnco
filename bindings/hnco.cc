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

#include "hnco/algorithms/all.hh"
#include "hnco/exception.hh"
#include "hnco/functions/all.hh"
#include "hnco/iterator.hh"
#include "hnco/map.hh"
#include "hnco/random.hh"

namespace py = pybind11;

using namespace hnco;

PYBIND11_MAKE_OPAQUE(bit_vector_t);
PYBIND11_MAKE_OPAQUE(algorithm::solution_t);

//
// Iterators
//

// For derived classes in pytonh
class PyIterator: public Iterator {
public:
  void init()                                   override { PYBIND11_OVERLOAD(void, Iterator, init, ); }
  bool has_next()                               override { PYBIND11_OVERLOAD_PURE(bool, Iterator, has_next, ); }
  const bit_vector_t& next()                    override { PYBIND11_OVERLOAD_PURE(const bit_vector_t&, Iterator, next, ); }
};

class HncoIterator
{
  Iterator& _iterator;
public:
  HncoIterator(Iterator& iterator)
    : _iterator(iterator)
  {
    _iterator.init();
  }
  const bit_vector_t& __next__() {
    if (_iterator.has_next())
      return _iterator.next();
    else
      throw py::stop_iteration();
  }
};

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
 
  py::class_<algorithm::solution_t>(module_hnco, "Solution")
    .def("__str__",
         [](const algorithm::solution_t& s) {
           std::ostringstream stream;
           bv_display(s.first, stream);
           stream << " -> " << s.second;
           return stream.str();
         })
    .def_readonly("first", &algorithm::solution_t::first)
    .def_readonly("second", &algorithm::solution_t::second)
    ;

  //
  // Iterators
  //

  py::class_<HncoIterator>(module_hnco, "HncoIterator")
    .def(py::init<Iterator&>())
    .def("__next__", &HncoIterator::__next__)
    ;

  py::class_<Iterator, PyIterator>(module_hnco, "Iterator")
    .def("init", &Iterator::init)
    .def("has_next", &Iterator::has_next)
    .def("next", &Iterator::next)
    .def("__iter__",
         [](Iterator& iterator) {
           return HncoIterator(iterator);
         }, py::keep_alive<0, 1>())
    ;

  py::class_<HypercubeIterator, Iterator>(module_hnco, "HypercubeIterator")
    .def(py::init<int>())
    ;

  //
  // Random numbers
  //

  py::module module_random = module_hnco.def_submodule("random", "Random numbers");

  {
    using namespace random;

    py::class_<Generator>(module_random, "Generator")
      .def_static("set_seed", static_cast<void (*)()>(&Generator::set_seed))
      .def_static("set_seed", static_cast<void (*)(unsigned)>(&Generator::set_seed))
      .def_static("reset", static_cast<void (*)()>(&Generator::reset))
      .def_static("uniform", static_cast<double (*)()>(&Generator::uniform))
      ;

  }

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

    py::register_exception<TargetReached>(module_hnco, "TargetReached");
    py::register_exception<LastEvaluation>(module_hnco, "LastEvaluation");

  }

  //
  // Functions
  //

  py::module module_function = module_hnco.def_submodule("function", "Functions");

  {
    using namespace hnco::function;

    py::class_<Function, PyFunction>(module_function, "Function")
      .def(py::init<>())
      .def("display", static_cast<void (Function::*)()>(&Function::display)) // Since Function::display is overloaded
      .def("get_bv_size", &Function::get_bv_size)
      .def("get_maximum", &Function::get_maximum)
      .def("has_known_maximum", &Function::has_known_maximum)
      .def("provides_incremental_evaluation", &Function::provides_incremental_evaluation)
      .def("evaluate", &Function::evaluate)
      ;

    py::class_<OneMax, Function>(module_function, "OneMax")
      .def(py::init<int>())
      ;

    py::class_<LinearFunction, Function>(module_function, "LinearFunction")
      .def(py::init<>())
      .def("random", &LinearFunction::random)
      .def("load", &LinearFunction::load)
      .def("save", &LinearFunction::save)
      ;

    py::class_<Decorator, Function>(module_function, "Decorator");

  }

  //
  // Controllers
  //

  py::module module_controller = module_function.def_submodule("controller", "Controllers");

  {
    using namespace function::controller;

    py::class_<Controller, function::Decorator>(module_controller, "Controller");

    py::class_<StopOnTarget, Controller>(module_controller, "StopOnTarget")
      .def(py::init<function::Function *, double>())
      .def("get_trigger", &StopOnTarget::get_trigger)
      ;

    py::class_<StopOnMaximum, StopOnTarget>(module_controller, "StopOnMaximum")
      .def(py::init<function::Function *>())
      ;

    py::class_<CallCounter, Controller>(module_controller, "CallCounter")
      .def(py::init<function::Function *>())
      .def("get_num_calls", &CallCounter::get_num_calls)
      ;

  }

  //
  // Modifiers
  //

  py::module module_modifier = module_function.def_submodule("modifier", "Modifiers");

  {
    using namespace function::modifier;

    py::class_<Modifier, function::Decorator>(module_modifier, "Modifier");

    py::class_<FunctionMapComposition, Modifier>(module_modifier, "FunctionMapComposition")
      .def(py::init<function::Function *, Map *>())
      ;

  }

  //
  // Algorithms
  //

  py::module module_algorithm = module_hnco.def_submodule("algorithm", "Algorithms");

  {
    using namespace hnco::algorithm;

    py::class_<Algorithm, PyAlgorithm>(module_algorithm, "Algorithm")
      .def(py::init<int>())
      .def("get_bv_size", &Algorithm::get_bv_size)
      .def("get_solution", &Algorithm::get_solution)
      .def("maximize", &Algorithm::maximize)
      .def("finalize", &Algorithm::finalize)
      ;

    py::class_<CompleteSearch, Algorithm>(module_algorithm, "CompleteSearch")
      .def(py::init<int>())
      ;

    py::class_<OnePlusOneEa, Algorithm>(module_algorithm, "OnePlusOneEa")
      .def(py::init<int>())
      .def("set_num_iterations", &OnePlusOneEa::set_num_iterations)
      .def("set_mutation_rate", &OnePlusOneEa::set_mutation_rate)
      .def("set_allow_no_mutation", &OnePlusOneEa::set_allow_no_mutation)
      .def("set_incremental_evaluation", &OnePlusOneEa::set_incremental_evaluation)
      ;

    py::class_<fast_efficient_p3::Hboa, Algorithm>(module_algorithm, "Hboa")
      .def(py::init<int>())
      .def("set_population_size", &fast_efficient_p3::Hboa::set_population_size)
      ;

    py::class_<fast_efficient_p3::Ltga, Algorithm>(module_algorithm, "Ltga")
      .def(py::init<int>())
      .def("set_population_size", &fast_efficient_p3::Ltga::set_population_size)
      ;

    py::class_<fast_efficient_p3::ParameterLessPopulationPyramid, Algorithm>(module_algorithm, "ParameterLessPopulationPyramid")
      .def(py::init<int>())
      ;

    py::class_<IterativeAlgorithm, Algorithm, PyIterativeAlgorithm>(module_algorithm, "IterativeAlgorithm")
      .def(py::init<int>())
      .def("set_num_iterations", &IterativeAlgorithm::set_num_iterations)
      ;

    py::class_<RandomSearch, IterativeAlgorithm>(module_algorithm, "RandomSearch")
      .def(py::init<int>())
      ;

  }

}
