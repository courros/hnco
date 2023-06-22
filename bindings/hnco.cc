/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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
#include <pybind11/complex.h>

#include "hnco/algorithms/all.hh"
#include "hnco/exception.hh"
#include "hnco/functions/all.hh"
#include "hnco/iterator.hh"
#include "hnco/maps/map.hh"
#include "hnco/multiobjective/algorithms/all.hh"
#include "hnco/multiobjective/functions/all.hh"
#include "hnco/neighborhoods/neighborhood.hh"
#include "hnco/random.hh"

namespace py = pybind11;

using namespace hnco;

PYBIND11_MAKE_OPAQUE(bit_vector_t);
PYBIND11_MAKE_OPAQUE(permutation_t);
PYBIND11_MAKE_OPAQUE(algorithm::solution_t);
PYBIND11_MAKE_OPAQUE(multiobjective::function::value_t);

//
// Iterators
//

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
// Neighborhoods
//

class PyNeighborhoodIterator: public neighborhood::NeighborhoodIterator {
public:
  using neighborhood::NeighborhoodIterator::NeighborhoodIterator;
  void set_origin(const bit_vector_t& x)        override { PYBIND11_OVERLOAD_PURE(void, neighborhood::NeighborhoodIterator, set_origin, ); }
};

class PyNeighborhood: public neighborhood::Neighborhood {
public:
  using neighborhood::Neighborhood::Neighborhood;
  void set_origin(const bit_vector_t& x)        override { PYBIND11_OVERLOAD_PURE(void, neighborhood::Neighborhood, set_origin, ); }
  const bit_vector_t& get_origin() const        override { PYBIND11_OVERLOAD_PURE(const bit_vector_t&, neighborhood::Neighborhood, get_origin, ); }
  const bit_vector_t& get_candidate() const     override { PYBIND11_OVERLOAD_PURE(const bit_vector_t&, neighborhood::Neighborhood, get_candidate, ); }
  const sparse_bit_vector_t& get_flipped_bits() const
                                                override { PYBIND11_OVERLOAD_PURE(const sparse_bit_vector_t&, neighborhood::Neighborhood, get_flipped_bits, ); }
  void propose()                                override { PYBIND11_OVERLOAD_PURE(void, neighborhood::Neighborhood, propose, ); }
  void keep()                                   override { PYBIND11_OVERLOAD_PURE(void, neighborhood::Neighborhood, keep, ); }
  void forget()                                 override { PYBIND11_OVERLOAD_PURE(void, neighborhood::Neighborhood, forget, ); }
  void mutate(bit_vector_t& bv)                 override { PYBIND11_OVERLOAD_PURE(void, neighborhood::Neighborhood, mutate, ); }
  void map(const bit_vector_t& input, bit_vector_t& output)
                                                override { PYBIND11_OVERLOAD_PURE(void, neighborhood::Neighborhood, map, ); }
};

//
// Functions
//

namespace hnco {
namespace function {

class PyFunction: public Function {
public:
  using Function::Function;
  int get_bv_size() const override {
    PYBIND11_OVERLOAD_PURE(int, Function, get_bv_size, );
  }
  double evaluate(const bit_vector_t& x) override {
    PYBIND11_OVERLOAD_PURE(double, Function, evaluate, x);
  }
  double get_maximum() const override {
    PYBIND11_OVERLOAD(double, Function, get_maximum, );
  }
  bool has_known_maximum() const override {
    PYBIND11_OVERLOAD(bool, Function, has_known_maximum, );
  }
  bool provides_incremental_evaluation() const override {
    PYBIND11_OVERLOAD(bool, Function, provides_incremental_evaluation, );
  }
};

class PyUniversalFunction: public UniversalFunction {
public:
  using UniversalFunction::UniversalFunction;
  double evaluate(const bit_vector_t& boolean_vars,
                  const std::vector<int>& integer_vars,
                  const std::vector<double>& float_vars,
                  const std::vector<std::complex<double>>& complex_vars,
                  const std::vector<int>& categorical_vars,
                  const std::vector<permutation_t>& permutation_vars) override {
    PYBIND11_OVERLOAD_PURE(double, UniversalFunction, evaluate,
                           boolean_vars, integer_vars, float_vars, complex_vars, categorical_vars, permutation_vars);
  }
};

} // end of namespace function
} // end of namespace hnco

//
// Algorithms
//

namespace hnco {
namespace algorithm {

class PyAlgorithm: public Algorithm {
public:
  using Algorithm::Algorithm;
  void maximize(const std::vector<function::Function *>& functions) override {
    PYBIND11_OVERLOAD_PURE(void, Algorithm, maximize, functions);
  }
  void finalize() override {
    PYBIND11_OVERLOAD(void, Algorithm, finalize, );
  }
};

class AlgorithmPublicist: public Algorithm {
public:
  using Algorithm::_solution;
  using Algorithm::random_solution;
  using Algorithm::update_solution;
};

class PyIterativeAlgorithm: public IterativeAlgorithm {
public:
  using IterativeAlgorithm::IterativeAlgorithm;
  void init() override {
    PYBIND11_OVERLOAD(void, IterativeAlgorithm, init, );
  }
  void iterate() override {
    PYBIND11_OVERLOAD_PURE(void, IterativeAlgorithm, iterate, );
  }
};

class IterativeAlgorithmPublicist: public IterativeAlgorithm {
public:
  using IterativeAlgorithm::init;
};

} // end of namespace algorithm
} // end of namespace hnco

//
// Multiobjective
//

namespace hnco {
namespace multiobjective {
namespace function {

class PyFunction: public Function {
public:
  using Function::Function;
  int get_bv_size() const override {
    PYBIND11_OVERLOAD_PURE(int, Function, get_bv_size, );
  }
  int get_output_size() const override {
    PYBIND11_OVERLOAD_PURE(int, Function, get_output_size, );
  }
  void evaluate(const bit_vector_t& bv, value_t& value) override {
    PYBIND11_OVERLOAD_PURE(void, Function, evaluate, bv, &value);
  }
};

class PyUniversalFunction: public UniversalFunction {
public:
  using UniversalFunction::UniversalFunction;
  int get_output_size() const override {
    PYBIND11_OVERLOAD_PURE(int, UniversalFunction, get_output_size, );
  }
  void evaluate(const bit_vector_t& boolean_vars,
                const std::vector<int>& integer_vars,
                const std::vector<double>& float_vars,
                const std::vector<std::complex<double>>& complex_vars,
                const std::vector<int>& categorical_vars,
                const std::vector<permutation_t> permutation_vars,
                value_t& value) override {
    PYBIND11_OVERLOAD_PURE(void, UniversalFunction, evaluate,
                           boolean_vars, integer_vars, float_vars, complex_vars, categorical_vars, permutation_vars, &value);
  }
};

} // end of namespace function
} // end of namespace multiobjective
} // end of namespace hnco

namespace hnco {
namespace multiobjective {
namespace algorithm {

class PyAlgorithm: public Algorithm {
public:
  using Algorithm::Algorithm;
  void minimize(const std::vector<function::Function *>& functions) override {
    PYBIND11_OVERLOAD_PURE(void, Algorithm, minimize, functions);
  }
  const Population& get_solutions() override {
    PYBIND11_OVERLOAD_PURE(const Population&, Algorithm, get_solutions, );
  }
};

class PyIterativeAlgorithm: public IterativeAlgorithm {
public:
  using IterativeAlgorithm::IterativeAlgorithm;
  void init() override { PYBIND11_OVERLOAD(void, IterativeAlgorithm, init, ); }
  void finalize() override { PYBIND11_OVERLOAD(void, IterativeAlgorithm, finalize, ); }
  void iterate() override { PYBIND11_OVERLOAD_PURE(void, IterativeAlgorithm, iterate, ); }
};

} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco

//
// Modules
//

PYBIND11_MODULE(hnco, module_hnco) {
 
  py::bind_vector<bit_vector_t>(module_hnco, "BitVector")
    .def(py::init<int>())
    .def(py::init<int, bit_t>())
    .def("__str__",
         [](const bit_vector_t& bv) {
           std::ostringstream stream;
           bv_display(bv, stream);
           return stream.str();
         })
    .def("random", [](bit_vector_t& bv) { bv_random(bv); })
    ;

  py::bind_vector<permutation_t>(module_hnco, "Permutation")
    .def(py::init<int>())
    .def("__str__",
         [](const permutation_t& permutation) {
           std::ostringstream stream;
           perm_display(permutation, stream);
           return stream.str();
         })
    .def("random", [](permutation_t& permutation) { perm_random(permutation); })
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

  module_hnco.def("bv_describe", [](const bit_vector_t& bv, function::Function *fn) { fn->describe(bv, std::cout); }, "Describe a bit vector in the context of a function");

  //
  // Neighborhoods
  //

  py::module module_neighborhood = module_hnco.def_submodule("neighborhood", "Neighborhoods");

  {
    using namespace neighborhood;

    py::class_<NeighborhoodIterator, Iterator, PyNeighborhoodIterator>(module_neighborhood, "NeighborhoodIterator")
      .def("set_origin", &NeighborhoodIterator::set_origin)
      ;

    py::class_<SingleBitFlipIterator, NeighborhoodIterator>(module_neighborhood, "SingleBitFlipIterator")
      .def(py::init<int>())
      ;

    py::class_<HammingSphereIterator, NeighborhoodIterator>(module_neighborhood, "HammingSphereIterator")
      .def(py::init<int, int>())
      ;

    py::class_<Neighborhood, PyNeighborhood>(module_neighborhood, "Neighborhood")
      .def("set_origin", &Neighborhood::set_origin)
      .def("get_origin", &Neighborhood::get_origin)
      .def("get_candidate", &Neighborhood::get_candidate)
      .def("get_flipped_bits", &Neighborhood::get_flipped_bits)
      .def("propose", &Neighborhood::propose)
      .def("keep", &Neighborhood::keep)
      .def("forget", &Neighborhood::forget)
      .def("map", &Neighborhood::map)
      .def("mutate", &Neighborhood::mutate)
      ;

    py::class_<SingleBitFlip, Neighborhood>(module_neighborhood, "SingleBitFlip")
      .def(py::init<int>())
      ;

    py::class_<HammingBall, Neighborhood>(module_neighborhood, "HammingBall")
      .def(py::init<int, int>())
      ;

    py::class_<HammingSphere, Neighborhood>(module_neighborhood, "HammingSphere")
      .def(py::init<int, int>())
      ;

    py::class_<StandardBitMutation, Neighborhood>(module_neighborhood, "StandardBitMutation")
      .def(py::init<int, double>())
      ;

  }

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

  py::module module_map = module_hnco.def_submodule("map", "Maps");

  {
    using namespace map;

    py::class_<Map>(module_map, "Map")
      .def("map", &Map::map)
      .def("get_input_size", &Map::get_input_size)
      .def("get_output_size", &Map::get_output_size)
      .def("is_surjective", &Map::is_surjective)
      .def("__str__",
           [](const Map& m) {
             std::ostringstream stream;
             m.display(stream);
             return stream.str();
           })
      ;

    py::class_<Translation, Map>(module_map, "Translation")
      .def(py::init<>())
      .def("random", &Translation::random)
      .def("load", static_cast<void (Translation::*)(std::string)>(&Translation::load))
      .def("save", static_cast<void (Translation::*)(std::string) const>(&Translation::save))
      ;

    py::class_<Permutation, Map>(module_map, "Permutation")
      .def(py::init<>())
      .def("random", &Permutation::random)
      .def("load", static_cast<void (Permutation::*)(std::string)>(&Permutation::load))
      .def("save", static_cast<void (Permutation::*)(std::string) const>(&Permutation::save))
      ;

    py::class_<LinearMap, Map>(module_map, "LinearMap")
      .def(py::init<>())
      .def("random", &LinearMap::random)
      .def("load", static_cast<void (LinearMap::*)(std::string)>(&LinearMap::load))
      .def("save", static_cast<void (LinearMap::*)(std::string) const>(&LinearMap::save))
      ;

    py::class_<AffineMap, Map>(module_map, "AffineMap")
      .def(py::init<>())
      .def("random", &AffineMap::random)
      .def("load", static_cast<void (AffineMap::*)(std::string)>(&AffineMap::load))
      .def("save", static_cast<void (AffineMap::*)(std::string) const>(&AffineMap::save))
      ;

    py::class_<MapComposition, Map>(module_map, "MapComposition")
      .def(py::init<>())
      .def(py::init<Map *, Map *>())
      ;

    py::class_<Injection, Map>(module_map, "Injection")
      .def(py::init<const std::vector<int>&, int>())
      ;

    py::class_<Projection, Map>(module_map, "Projection")
      .def(py::init<const std::vector<int>&, int>())
      ;

    py::class_<TsAffineMap, Map> ts_affine_map(module_map, "TsAffineMap");

    ts_affine_map
      .def(py::init<>())
      .def("random", &TsAffineMap::random)
      .def("load", static_cast<void (TsAffineMap::*)(std::string)>(&TsAffineMap::load))
      .def("save", static_cast<void (TsAffineMap::*)(std::string) const>(&TsAffineMap::save))
      ;

    py::enum_<TsAffineMap::SamplingMode>(ts_affine_map, "SamplingMode")
      .value("Unconstrained", TsAffineMap::SamplingMode::Unconstrained)
      .value("CommutingTransvections", TsAffineMap::SamplingMode::CommutingTransvections)
      .value("UniqueSource", TsAffineMap::SamplingMode::UniqueSource)
      .value("UniqueDestination", TsAffineMap::SamplingMode::UniqueDestination)
      .value("DisjointTransvections", TsAffineMap::SamplingMode::DisjointTransvections)
      .value("NonCommutingTransvections", TsAffineMap::SamplingMode::NonCommutingTransvections)
      .export_values();
  }

  //
  // Exceptions
  //

  {
    using namespace exception;

    py::register_exception<TargetReached>(module_hnco, "TargetReached");
    py::register_exception<LastEvaluation>(module_hnco, "LastEvaluation");

  }

  //
  // Functions
  //

  py::module module_function = module_hnco.def_submodule("function", "Functions");

  {
    using namespace function;

    py::class_<Function, PyFunction>(module_function, "Function")
      .def(py::init<>())
      .def("get_bv_size", &Function::get_bv_size)
      .def("get_maximum", &Function::get_maximum)
      .def("has_known_maximum", &Function::has_known_maximum)
      .def("provides_incremental_evaluation", &Function::provides_incremental_evaluation)
      .def("evaluate", &Function::evaluate)
      .def("__str__",
           [](Function& fn) {
             std::ostringstream stream;
             fn.display(stream);
             return stream.str();
           })
      ;

    py::class_<AbstractMaxSat, Function>(module_function, "AbstractMaxSat")
      .def("load", &AbstractMaxSat::load)
      .def("save", &AbstractMaxSat::save)
      ;

    py::class_<MaxSat, AbstractMaxSat>(module_function, "MaxSat")
      .def(py::init<>())
      .def("random", static_cast<void (MaxSat::*)(int, int, int)>(&MaxSat::random))
      .def("random", static_cast<void (MaxSat::*)(const bit_vector_t&, int, int)>(&MaxSat::random))
      ;

    py::class_<MaxNae3Sat, AbstractMaxSat>(module_function, "MaxNae3Sat")
      .def(py::init<>())
      ;

    py::class_<DeceptiveJump, Function>(module_function, "DeceptiveJump")
      .def(py::init<int, int>())
      ;

    py::class_<EqualProducts, Function>(module_function, "EqualProducts")
      .def(py::init<>())
      .def("random", &EqualProducts::random)
      .def("load", &EqualProducts::load)
      .def("save", &EqualProducts::save)
      ;

#ifdef ENABLE_FACTORIZATION
    py::class_<Factorization, Function>(module_function, "Factorization")
      .def(py::init<>())
      .def(py::init<const std::string>())
      .def("load", &Factorization::load)
      ;
#endif

    py::class_<FourPeaks, Function>(module_function, "FourPeaks")
      .def(py::init<int, int>())
      ;

#ifdef ENABLE_PLUGIN
    py::class_<FunctionPlugin, Function>(module_function, "FunctionPlugin")
      .def(py::init<int, std::string, std::string>())
      ;
#endif

    py::class_<Hiff, Function>(module_function, "Hiff")
      .def(py::init<int>())
      ;

    py::class_<Jump, Function>(module_function, "Jump")
      .def(py::init<int, int>())
      ;

    py::class_<Labs, Function>(module_function, "Labs")
      .def(py::init<int>())
      .def("set_merit_factor_flag", &Labs::set_merit_factor_flag)
      ;

    py::class_<LeadingOnes, Function>(module_function, "LeadingOnes")
      .def(py::init<int>())
      ;

    py::class_<LinearFunction, Function>(module_function, "LinearFunction")
      .def(py::init<>())
      .def("random", &LinearFunction::random)
      .def("load", &LinearFunction::load)
      .def("save", &LinearFunction::save)
      ;

    py::class_<LongPath, Function>(module_function, "LongPath")
      .def(py::init<int, int>())
      ;

    py::class_<NearestNeighborIsingModel1, Function>(module_function, "NearestNeighborIsingModel1")
      .def(py::init<>())
      .def("random", &NearestNeighborIsingModel1::random)
      .def("load", static_cast<void (NearestNeighborIsingModel1::*)(std::string)>(&NearestNeighborIsingModel1::load))
      .def("save", static_cast<void (NearestNeighborIsingModel1::*)(std::string) const>(&NearestNeighborIsingModel1::save))
      .def("set_periodic_boundary_conditions", &NearestNeighborIsingModel1::set_periodic_boundary_conditions)
      ;

    py::class_<NearestNeighborIsingModel2, Function>(module_function, "NearestNeighborIsingModel2")
      .def(py::init<>())
      .def("random", &NearestNeighborIsingModel2::random)
      .def("load", static_cast<void (NearestNeighborIsingModel2::*)(std::string)>(&NearestNeighborIsingModel2::load))
      .def("save", static_cast<void (NearestNeighborIsingModel2::*)(std::string) const>(&NearestNeighborIsingModel2::save))
      .def("set_periodic_boundary_conditions", &NearestNeighborIsingModel2::set_periodic_boundary_conditions)
      ;

    py::class_<Needle, Function>(module_function, "Needle")
      .def(py::init<int>())
      ;

    py::class_<NkLandscape, Function>(module_function, "NkLandscape")
      .def(py::init<>())
      .def("random", &NkLandscape::random)
      .def("load", static_cast<void (NkLandscape::*)(std::string)>(&NkLandscape::load))
      .def("save", static_cast<void (NkLandscape::*)(std::string) const>(&NkLandscape::save))
      ;

    py::class_<Partition, Function>(module_function, "Partition")
      .def(py::init<>())
      .def("random", &Partition::random)
      .def("load", static_cast<void (Partition::*)(std::string)>(&Partition::load))
      .def("save", static_cast<void (Partition::*)(std::string) const>(&Partition::save))
      ;

    py::class_<OneMax, Function>(module_function, "OneMax")
      .def(py::init<int>())
      ;

    py::class_<Plateau, Function>(module_function, "Plateau")
      .def(py::init<int>())
      ;

    py::class_<Qubo, Function>(module_function, "Qubo")
      .def(py::init<>())
      .def("load", static_cast<void (Qubo::*)(std::string)>(&Qubo::load))
      ;

    py::class_<Ridge, Function>(module_function, "Ridge")
      .def(py::init<int>())
      ;

    py::class_<SixPeaks, Function>(module_function, "SixPeaks")
      .def(py::init<int, int>())
      ;

    py::class_<SummationCancellation, Function>(module_function, "SummationCancellation")
      .def(py::init<int>())
      ;

    py::class_<SinusSummationCancellation, SummationCancellation>(module_function, "SinusSummationCancellation")
      .def(py::init<int>())
      ;

    py::class_<Trap, Function>(module_function, "Trap")
      .def(py::init<int, int>())
      ;

    py::class_<WalshExpansion, Function>(module_function, "WalshExpansion")
      .def(py::init<>())
      .def("random", &WalshExpansion::random)
      .def("load", static_cast<void (WalshExpansion::*)(std::string)>(&WalshExpansion::load))
      .def("save", static_cast<void (WalshExpansion::*)(std::string) const>(&WalshExpansion::save))
      ;

    py::class_<WalshExpansion1, Function>(module_function, "WalshExpansion1")
      .def(py::init<>())
      .def("random", &WalshExpansion1::random)
      .def("load", static_cast<void (WalshExpansion1::*)(std::string)>(&WalshExpansion1::load))
      .def("save", static_cast<void (WalshExpansion1::*)(std::string) const>(&WalshExpansion1::save))
      ;

    py::class_<WalshExpansion2, Function>(module_function, "WalshExpansion2")
      .def(py::init<>())
      .def("random", &WalshExpansion2::random)
      .def("load", static_cast<void (WalshExpansion2::*)(std::string)>(&WalshExpansion2::load))
      .def("save", static_cast<void (WalshExpansion2::*)(std::string) const>(&WalshExpansion2::save))
      ;

    py::class_<Decorator, Function>(module_function, "Decorator");

    using namespace representation;

    using IntegerRep = DyadicIntegerRepresentation<int>;
    using FloatRep = DyadicFloatRepresentation<double>;
    using ComplexRep = DyadicComplexRepresentation<double>;

    py::class_<UniversalFunctionAdapter, Function>(module_function, "UniversalFunctionAdapter")
      .def(py::init<
           UniversalFunction *,
           int,
           std::vector<IntegerRep>,
           std::vector<FloatRep>,
           std::vector<ComplexRep>,
           std::vector<LinearCategoricalRepresentation>,
           std::vector<PermutationRepresentation>>())
      ;

    py::class_<UniversalFunction, PyUniversalFunction>(module_function, "UniversalFunction")
      .def(py::init<>())
      .def("evaluate", &UniversalFunction::evaluate)
      .def("__str__",
           [](UniversalFunction& fn) {
             std::ostringstream stream;
             fn.display(stream);
             return stream.str();
           })
      ;

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

    py::class_<OnBudgetFunction, CallCounter>(module_controller, "OnBudgetFunction")
      .def(py::init<function::Function *, int>())
      ;

    py::class_<ProgressTracker::Event>(module_controller, "Event")
      ;

    py::class_<ProgressTracker, CallCounter>(module_controller, "ProgressTracker")
      .def(py::init<function::Function *>())
      .def("set_log_improvement", &ProgressTracker::set_log_improvement)
      .def("set_record_evaluation_time", &ProgressTracker::set_record_evaluation_time)
      .def("get_evaluation_time", &ProgressTracker::get_evaluation_time)
      .def("get_last_improvement", &ProgressTracker::get_last_improvement)
      ;

    py::class_<Cache, Controller>(module_controller, "Cache")
      .def(py::init<function::Function *>())
      .def("get_lookup_ratio", &Cache::get_lookup_ratio)
      ;

  }

  //
  // Modifiers
  //

  py::module module_modifier = module_function.def_submodule("modifier", "Modifiers");

  {
    using namespace function::modifier;
    using namespace map;

    py::class_<Modifier, function::Decorator>(module_modifier, "Modifier");

    py::class_<AdditiveGaussianNoise, Modifier>(module_modifier, "AdditiveGaussianNoise")
      .def(py::init<function::Function *, double>())
      ;

    py::class_<FunctionMapComposition, Modifier>(module_modifier, "FunctionMapComposition")
      .def(py::init<function::Function *, Map *>())
      ;

    py::class_<Negation, Modifier>(module_modifier, "Negation")
      .def(py::init<function::Function *>())
      ;

    py::class_<ParsedModifier, Modifier>(module_modifier, "ParsedModifier")
      .def(py::init<function::Function *, std::string>())
      ;

    py::class_<PriorNoise, Modifier>(module_modifier, "PriorNoise")
      .def(py::init<function::Function *, neighborhood::Neighborhood *>())
      ;

  }

  //
  // Representations
  //

  py::module module_representation = module_hnco.def_submodule("representation", "Representations");

  {
    using namespace function;
    using namespace representation;

    using IntegerRep = DyadicIntegerRepresentation<int>;
    using FloatRep = DyadicFloatRepresentation<double>;
    using ComplexRep = DyadicComplexRepresentation<double>;

    py::class_<IntegerRep>(module_representation, "DyadicIntegerRepresentation")
      .def(py::init<int, int, int>())
      .def(py::init<int, int>())
      .def("size", &IntegerRep::size)
      .def("unpack", &IntegerRep::unpack)
      .def("__str__",
           [](const IntegerRep& rep) {
             std::ostringstream stream;
             rep.display(stream);
             return stream.str();
           })
      ;

    py::class_<FloatRep>(module_representation, "DyadicFloatRepresentation")
      .def(py::init<double, double, int>())
      .def(py::init<double, double, double>())
      .def("size", &FloatRep::size)
      .def("unpack", &FloatRep::unpack)
      .def("__str__",
           [](const FloatRep& rep) {
             std::ostringstream stream;
             rep.display(stream);
             return stream.str();
           })
      ;

    py::class_<ComplexRep>(module_representation, "DyadicComplexRepresentation")
      .def(py::init<FloatRep, FloatRep>())
      .def(py::init<FloatRep>())
      .def("size", &ComplexRep::size)
      .def("unpack", &ComplexRep::unpack)
      .def("__str__",
           [](const ComplexRep& rep) {
             std::ostringstream stream;
             rep.display(stream);
             return stream.str();
           })
      ;

    py::class_<LinearCategoricalRepresentation>(module_representation, "LinearCategoricalRepresentation")
      .def(py::init<int>())
      .def("size", &LinearCategoricalRepresentation::size)
      .def("unpack", &LinearCategoricalRepresentation::unpack)
      .def("__str__",
           [](const LinearCategoricalRepresentation& rep) {
             std::ostringstream stream;
             rep.display(stream);
             return stream.str();
           })
      ;

    py::class_<PermutationRepresentation>(module_representation, "PermutationRepresentation")
      .def(py::init<int, int>())
      .def("size", &PermutationRepresentation::size)
      .def("unpack", &PermutationRepresentation::unpack)
      .def("__str__",
           [](const PermutationRepresentation& rep) {
             std::ostringstream stream;
             rep.display(stream);
             return stream.str();
           })
      ;

  }

  //
  // Algorithms
  //

  py::module module_algorithm = module_hnco.def_submodule("algorithm", "Algorithms");

  {
    using namespace algorithm;

    py::class_<Algorithm, PyAlgorithm>(module_algorithm, "Algorithm")
      .def(py::init<int>())
      .def("get_solution", &Algorithm::get_solution)
      .def("maximize", &Algorithm::maximize)
      .def("finalize", &Algorithm::finalize)
      .def("random_solution", &AlgorithmPublicist::random_solution)
      .def("update_solution", py::overload_cast<const bit_vector_t&>(&AlgorithmPublicist::update_solution))
      .def_readwrite("solution", &AlgorithmPublicist::_solution)
      ;

    py::class_<CompleteSearch, Algorithm>(module_algorithm, "CompleteSearch")
      .def(py::init<int>())
      ;

    py::class_<IterativeAlgorithm, Algorithm, PyIterativeAlgorithm>(module_algorithm, "IterativeAlgorithm")
      .def(py::init<int>())
      .def("init", &IterativeAlgorithmPublicist::init)
      .def("set_num_iterations", &IterativeAlgorithm::set_num_iterations)
      ;

    py::class_<RandomSearch, IterativeAlgorithm>(module_algorithm, "RandomSearch")
      .def(py::init<int>())
      ;

    py::class_<LocalSearchAlgorithm<neighborhood::Neighborhood>, IterativeAlgorithm>(module_algorithm, "LocalSearchAlgorithm");

    py::class_<RandomWalk, LocalSearchAlgorithm<neighborhood::Neighborhood>>(module_algorithm, "RandomWalk")
      .def(py::init<int, neighborhood::Neighborhood *>())
      ;

    py::class_<RandomLocalSearch, LocalSearchAlgorithm<neighborhood::Neighborhood>>(module_algorithm, "RandomLocalSearch")
      .def(py::init<int, neighborhood::Neighborhood *>())
      .def("set_patience", &RandomLocalSearch::set_patience)
      ;

    py::class_<SimulatedAnnealing, LocalSearchAlgorithm<neighborhood::Neighborhood>>(module_algorithm, "SimulatedAnnealing")
      .def(py::init<int, neighborhood::Neighborhood *>())
      .def("set_num_transitions", &SimulatedAnnealing::set_num_transitions)
      .def("set_num_trials", &SimulatedAnnealing::set_num_trials)
      .def("set_initial_acceptance_probability", &SimulatedAnnealing::set_initial_acceptance_probability)
      .def("set_beta_ratio", &SimulatedAnnealing::set_beta_ratio)
      ;

    py::class_<OnePlusOneEa, Algorithm>(module_algorithm, "OnePlusOneEa")
      .def(py::init<int>())
      .def("set_num_iterations", &OnePlusOneEa::set_num_iterations)
      .def("set_mutation_rate", &OnePlusOneEa::set_mutation_rate)
      .def("set_allow_no_mutation", &OnePlusOneEa::set_allow_no_mutation)
      .def("set_incremental_evaluation", &OnePlusOneEa::set_incremental_evaluation)
      ;

    py::class_<SelfAdjustingOnePlusOneEa, IterativeAlgorithm>(module_algorithm, "SelfAdjustingOnePlusOneEa")
      .def(py::init<int>())
      .def("set_num_iterations", &SelfAdjustingOnePlusOneEa::set_num_iterations)
      .def("set_mutation_rate_init", &SelfAdjustingOnePlusOneEa::set_mutation_rate_init)
      .def("set_mutation_rate_min", &SelfAdjustingOnePlusOneEa::set_mutation_rate_min)
      .def("set_mutation_rate_max", &SelfAdjustingOnePlusOneEa::set_mutation_rate_max)
      .def("set_update_strength", &SelfAdjustingOnePlusOneEa::set_update_strength)
      .def("set_success_ratio", &SelfAdjustingOnePlusOneEa::set_success_ratio)
      .def("set_allow_no_mutation", &SelfAdjustingOnePlusOneEa::set_allow_no_mutation)
      .def("set_incremental_evaluation", &SelfAdjustingOnePlusOneEa::set_incremental_evaluation)
      ;

    py::class_<MuPlusLambdaEa, IterativeAlgorithm>(module_algorithm, "MuPlusLambdaEa")
      .def(py::init<int, int, int>())
      .def("set_mutation_rate", &MuPlusLambdaEa::set_mutation_rate)
      .def("set_allow_no_mutation", &MuPlusLambdaEa::set_allow_no_mutation)
      ;

    py::class_<MuCommaLambdaEa, IterativeAlgorithm>(module_algorithm, "MuCommaLambdaEa")
      .def(py::init<int, int, int>())
      .def("set_mutation_rate", &MuCommaLambdaEa::set_mutation_rate)
      .def("set_allow_no_mutation", &MuCommaLambdaEa::set_allow_no_mutation)
      ;

    py::class_<TwoRateOnePlusLambdaEa, IterativeAlgorithm>(module_algorithm, "TwoRateOnePlusLambdaEa")
      .def(py::init<int, int>())
      .def("set_mutation_rate_init", &TwoRateOnePlusLambdaEa::set_mutation_rate_init)
      .def("set_allow_no_mutation", &TwoRateOnePlusLambdaEa::set_allow_no_mutation)
      ;

    py::class_<InformationTheoreticEa, IterativeAlgorithm>(module_algorithm, "InformationTheoreticEa")
      .def(py::init<int, int>())
      .def("set_allow_no_mutation", &InformationTheoreticEa::set_allow_no_mutation)
      .def("set_initial_hamming_weight", &InformationTheoreticEa::set_initial_hamming_weight)
      .def("set_learning_rate", &InformationTheoreticEa::set_learning_rate)
      .def("set_mutation_rate_init", &InformationTheoreticEa::set_mutation_rate_init)
      .def("set_mutation_rate_max", &InformationTheoreticEa::set_mutation_rate_max)
      .def("set_mutation_rate_min", &InformationTheoreticEa::set_mutation_rate_min)
      .def("set_replacement", &InformationTheoreticEa::set_replacement)
      .def("set_selection_size", &InformationTheoreticEa::set_selection_size)
      ;

    py::class_<GeneticAlgorithm, IterativeAlgorithm>(module_algorithm, "GeneticAlgorithm")
      .def(py::init<int, int>())
      .def("set_mutation_rate", &GeneticAlgorithm::set_mutation_rate)
      .def("set_crossover_probability", &GeneticAlgorithm::set_crossover_probability)
      .def("set_tournament_size", &GeneticAlgorithm::set_tournament_size)
      .def("set_allow_no_mutation", &GeneticAlgorithm::set_allow_no_mutation)
      ;

    py::class_<OnePlusLambdaCommaLambdaGa, IterativeAlgorithm>(module_algorithm, "OnePlusLambdaCommaLambdaGa")
      .def(py::init<int, int>())
      .def("set_mutation_rate", &OnePlusLambdaCommaLambdaGa::set_mutation_rate)
      .def("set_crossover_bias", &OnePlusLambdaCommaLambdaGa::set_crossover_bias)
      ;

    py::class_<PvAlgorithm, IterativeAlgorithm>(module_algorithm, "PvAlgorithm")
      ;

    py::class_<Pbil, PvAlgorithm>(module_algorithm, "Pbil")
      .def(py::init<int, int>())
      .def("set_learning_rate", &Pbil::set_learning_rate)
      .def("set_selection_size", &Pbil::set_selection_size)
      ;

    py::class_<NpsPbil, PvAlgorithm>(module_algorithm, "NpsPbil")
      .def(py::init<int, int>())
      .def("set_learning_rate", &NpsPbil::set_learning_rate)
      .def("set_selection_size", &NpsPbil::set_selection_size)
      ;

    py::class_<Umda, PvAlgorithm>(module_algorithm, "Umda")
      .def(py::init<int, int>())
      .def("set_selection_size", &Umda::set_selection_size)
      ;

    py::class_<CompactGa, PvAlgorithm>(module_algorithm, "CompactGa")
      .def(py::init<int>())
      .def("set_learning_rate", &CompactGa::set_learning_rate)
      ;

    py::class_<Mmas, PvAlgorithm>(module_algorithm, "Mmas")
      .def(py::init<int>())
      .def("set_learning_rate", &Mmas::set_learning_rate)
      ;

    {
      using namespace algorithm::walsh_moment;

      {
        using Algo = Hea<SymmetricWalshMoment2Herding>;
        py::class_<Algo, IterativeAlgorithm>(module_algorithm, "Hea")
          .def(py::init<int, int>())
          .def("set_bound_moment", &Algo::set_bound_moment)
          .def("set_learning_rate", &Algo::set_learning_rate)
          .def("set_num_iterations", &Algo::set_num_iterations)
          .def("set_randomize_bit_order", &Algo::set_randomize_bit_order)
          .def("set_reset_period", &Algo::set_reset_period)
          .def("set_selection_size", &Algo::set_selection_size)
          ;
      }

      {
        using Algo = BmPbil<SymmetricWalshMoment2GibbsSampler>;
        py::class_<Algo, IterativeAlgorithm>(module_algorithm, "BmPbil")
          .def(py::init<int, int>())
          .def("set_learning_rate", &Algo::set_learning_rate)
          .def("set_mc_reset_strategy", &Algo::set_mc_reset_strategy)
          .def("set_negative_positive_selection", &Algo::set_negative_positive_selection)
          .def("set_num_gs_cycles", &Algo::set_num_gs_cycles)
          .def("set_num_gs_steps", &Algo::set_num_gs_steps)
          .def("set_num_iterations", &Algo::set_num_iterations)
          .def("set_sampling", &Algo::set_sampling)
          .def("set_selection_size", &Algo::set_selection_size)
          ;
      }

    }

    py::class_<Mimic, IterativeAlgorithm>(module_algorithm, "Mimic")
      .def(py::init<int, int>())
      .def("set_selection_size", &Mimic::set_selection_size)
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

    py::class_<Population>(module_algorithm, "Population")
      .def(py::init<int, int>())

      .def("get_size", &Population::get_size)
      .def("get_bv_size", &Population::get_bv_size)

      .def("random", &Population::random)

      .def("get_best_bv", static_cast<bit_vector_t& (Population::*)()>(&Population::get_best_bv))
      .def("get_best_bv", static_cast<bit_vector_t& (Population::*)(int)>(&Population::get_best_bv))
      .def("get_worst_bv", &Population::get_worst_bv)

      .def("get_best_value", static_cast<double (Population::*)() const>(&Population::get_best_value))
      .def("get_best_value", static_cast<double (Population::*)(int) const>(&Population::get_best_value))

      .def("evaluate", &Population::evaluate)
      .def("sort", &Population::sort)
      .def("partial_sort", &Population::partial_sort)

      .def("get_equivalent_bvs", &Population::get_equivalent_bvs)

      .def_readwrite("bvs", &Population::bvs)
      .def_readwrite("values", &Population::values)
      ;

  }

  //
  // Multiobjective
  //

  py::module module_multiobjective = module_hnco.def_submodule("multiobjective", "Multiobjective optimization");
  py::module module_multiobjective_function = module_multiobjective.def_submodule("function", "Functions for multiobjective optimization");
  py::module module_multiobjective_algorithm = module_multiobjective.def_submodule("algorithm", "Algorithms for multiobjective optimization");

  {
    using namespace multiobjective::function;

    py::bind_vector<value_t>(module_multiobjective_function, "Value")
      .def(py::init<int>())
      .def(py::init<int, double>())
      .def("__str__",
           [](const value_t& value) {
             std::ostringstream stream;
             value_display(value, stream);
             return stream.str();
           })
      ;

    py::class_<Function, multiobjective::function::PyFunction>(module_multiobjective_function, "Function")
      .def(py::init<>())
      .def("get_bv_size", &Function::get_bv_size)
      .def("get_output_size", &Function::get_output_size)
      .def("evaluate", &Function::evaluate)
      .def("__str__",
           [](Function& fn) {
             std::ostringstream stream;
             fn.display(stream);
             return stream.str();
           })
      ;

    module_multiobjective_function.def("bv_describe", [](const bit_vector_t& bv, multiobjective::function::Function *fn) { fn->describe(bv, std::cout); }, "Describe a bit vector in the context of a function");

    using namespace representation;

    using IntegerRep = DyadicIntegerRepresentation<int>;
    using FloatRep = DyadicFloatRepresentation<double>;
    using ComplexRep = DyadicComplexRepresentation<double>;

    py::class_<UniversalFunctionAdapter, Function>(module_multiobjective_function, "UniversalFunctionAdapter")
      .def(py::init<
           UniversalFunction *,
           int,
           std::vector<IntegerRep>,
           std::vector<FloatRep>,
           std::vector<ComplexRep>,
           std::vector<LinearCategoricalRepresentation>,
           std::vector<PermutationRepresentation>>())
      ;

    py::class_<UniversalFunction, PyUniversalFunction>(module_multiobjective_function, "UniversalFunction")
      .def(py::init<>())
      .def("evaluate", &UniversalFunction::evaluate)
      .def("__str__",
           [](UniversalFunction& fn) {
             std::ostringstream stream;
             fn.display(stream);
             return stream.str();
           })
      ;

  }

  {
    using namespace multiobjective::algorithm;

    py::class_<Population>(module_multiobjective_algorithm, "Population")
      .def(py::init<int, int, int>())
      .def("get_size", &Population::get_size)
      .def("random", &Population::random)
      .def("evaluate", &Population::evaluate)
      .def_readwrite("bvs", &Population::bvs)
      .def_readwrite("values", &Population::values)
      ;

    py::class_<Algorithm, multiobjective::algorithm::PyAlgorithm>(module_multiobjective_algorithm, "Algorithm")
      .def(py::init<int, int>())
      .def("get_solutions", &Algorithm::get_solutions, py::return_value_policy::reference)
      .def("minimize", &Algorithm::minimize)
      ;

    py::class_<IterativeAlgorithm, Algorithm, multiobjective::algorithm::PyIterativeAlgorithm>(module_multiobjective_algorithm, "IterativeAlgorithm")
      .def("set_num_iterations", &IterativeAlgorithm::set_num_iterations)
      ;

    py::class_<Nsga2, IterativeAlgorithm>(module_multiobjective_algorithm, "Nsga2")
      .def(py::init<int, int, int>())
      .def("set_tournament_size", &Nsga2::set_tournament_size)
      .def("set_mutation_rate", &Nsga2::set_mutation_rate)
      .def("set_allow_no_mutation", &Nsga2::set_allow_no_mutation)
      .def("set_crossover_probability", &Nsga2::set_crossover_probability)
      ;

  }

}
