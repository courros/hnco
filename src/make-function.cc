/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#include "config.h"

#include "hnco/functions/all.hh"
#include "hnco/functions/decorators/all.hh"
#include "hnco/neighborhoods/neighborhood.hh"

#include "make-function.hh"

using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::neighborhood;
using namespace hnco;


Function *
make_concrete_function(const Options& options)
{
  if (options.get_bv_size() <= 0)
    throw Error("make_concrete_function: bv_size must be positive");

  switch(options.get_function()) {

  case 0:
    return new OneMax
      (options.get_bv_size());

  case 1: {
    LinearFunction* function = new LinearFunction;
    std::ifstream ifs(options.get_path());
    if (!ifs.good()) {
      std::ostringstream stream;
      stream << "make_concrete_function (LinearFunction): Cannot open " << options.get_path();
      throw Error(stream.str());
    }
    try {
      boost::archive::text_iarchive ia(ifs);
      ia >> (*function);
    }
    catch (boost::archive::archive_exception& e) {
      throw Error(e.what());
    }
    return function;
  }

  case 10:
    return new LeadingOnes
      (options.get_bv_size());

  case 11:
    return new Ridge
      (options.get_bv_size());

  case 20:
    return new Needle
      (options.get_bv_size());

  case 30:
    return new Jump
      (options.get_bv_size(),
       options.get_fn_threshold());

  case 31:
    return new DeceptiveJump
      (options.get_bv_size(),
       options.get_fn_threshold());

  case 40:
    return new FourPeaks
      (options.get_bv_size(),
       options.get_fn_threshold());

  case 41:
    return new SixPeaks
      (options.get_bv_size(),
       options.get_fn_threshold());

  case 50: {
    std::ifstream ifs(options.get_path());
    if (!ifs.good()) {
      std::ostringstream stream;
      stream << "make_concrete_function (Qubo): Cannot open " << options.get_path();
      throw Error(stream.str());
    }
    Qubo* function = new Qubo;
    function->load(ifs);
    return function;
  }

  case 60: {
    NkLandscape* function = new NkLandscape;
    std::ifstream ifs(options.get_path());
    if (!ifs.good()) {
      std::ostringstream stream;
      stream << "make_concrete_function (NkLandscape): Cannot open " << options.get_path();
      throw Error(stream.str());
    }
    try {
      boost::archive::text_iarchive ia(ifs);
      ia >> (*function);
    }
    catch (boost::archive::archive_exception& e) {
      throw Error(e.what());
    }
    return function;
  }

  case 70: {
    std::ifstream ifs(options.get_path());
    if (!ifs.good()) {
      std::ostringstream stream;
      stream << "make_concrete_function (MaxSat): Cannot open " << options.get_path();
      throw Error(stream.str());
    }
    MaxSat *function = new MaxSat;
    function->load(ifs);
    return function;
  }

  case 80:
    return new Labs
      (options.get_bv_size());

  case 81:
    return new LabsMeritFactor
      (options.get_bv_size());

  case 90: {
    EqualProducts* function = new EqualProducts;
    std::ifstream ifs(options.get_path());
    if (!ifs.good()) {
      std::ostringstream stream;
      stream << "make_concrete_function (EqualProducts): Cannot open " << options.get_path();
      throw Error(stream.str());
    }
    try {
      boost::archive::text_iarchive ia(ifs);
      ia >> (*function);
    }
    catch (boost::archive::archive_exception& e) {
      throw Error(e.what());
    }
    return function;
  }

  case 100:
    return new SummationCancellation
      (options.get_bv_size());

  case 101:
    return new SinusSummationCancellation
      (options.get_bv_size());

  case 110:
    return new Trap
      (options.get_bv_size(),
       options.get_fn_num_traps());

  case 120:
    return new Hiff
      (options.get_bv_size());

  case 130:
    return new Plateau
      (options.get_bv_size());

  case 140:
    return new LongPath
      (options.get_bv_size(),
       options.get_fn_prefix_length());

#ifdef ENABLE_FACTORIZATION
  case 150:
    return new Factorization
      (options.get_path());
#endif

  case 160: {
    WalshExpansion* function = new WalshExpansion;
    std::ifstream ifs(options.get_path());
    if (!ifs.good()) {
      std::ostringstream stream;
      stream << "make_concrete_function (WalshExpansion): Cannot open " << options.get_path();
      throw Error(stream.str());
    }
    try {
      boost::archive::text_iarchive ia(ifs);
      ia >> (*function);
    }
    catch (boost::archive::archive_exception& e) {
      throw Error(e.what());
    }
    return function;
  }

  case 161: {
    WalshExpansion1* function = new WalshExpansion1;
    std::ifstream ifs(options.get_path());
    if (!ifs.good()) {
      std::ostringstream stream;
      stream << "make_concrete_function (WalshExpansion1): Cannot open " << options.get_path();
      throw Error(stream.str());
    }
    try {
      boost::archive::text_iarchive ia(ifs);
      ia >> (*function);
    }
    catch (boost::archive::archive_exception& e) {
      throw Error(e.what());
    }
    return function;
  }

  case 162: {
    WalshExpansion2* function = new WalshExpansion2;
    std::ifstream ifs(options.get_path());
    if (!ifs.good()) {
      std::ostringstream stream;
      stream << "make_concrete_function (WalshExpansion2): Cannot open " << options.get_path();
      throw Error(stream.str());
    }
    try {
      boost::archive::text_iarchive ia(ifs);
      ia >> (*function);
    }
    catch (boost::archive::archive_exception& e) {
      throw Error(e.what());
    }
    return function;
  }

#ifdef ENABLE_PLUGIN
  case 1000:
    return new FunctionPlugin
      (options.get_bv_size(),
       options.get_path(),
       options.get_fn_name());
#endif

  default:
    std::ostringstream stream;
    stream << "make_concrete_function: Unknown function type: " << options.get_function();
    throw Error(stream.str());
  }

}


Map *
make_map(const Options& options)
{
  assert(options.get_map() > 0);

  switch(options.get_map()) {

  case 1: {
    Translation *map = new Translation;
    if (options.with_map_random()) {
      map->random(options.get_bv_size());
    } else {
      std::ifstream ifs(options.get_map_path());
      if (!ifs.good()) {
        std::ostringstream stream;
        stream << "make_map (Translation): Cannot open " << options.get_map_path();
        throw Error(stream.str());
      }
      try {
        boost::archive::text_iarchive ia(ifs);
        ia >> (*map);
      }
      catch (boost::archive::archive_exception& e) {
        throw Error(e.what());
      }
    }
    return map;
  }

  case 2: {
    Permutation *map = new Permutation;
    if (options.with_map_random()) {
      map->random(options.get_bv_size());
    } else {
      std::ifstream ifs(options.get_map_path());
      if (!ifs.good()) {
        std::ostringstream stream;
        stream << "make_map (Permutation): Cannot open " << options.get_map_path();
        throw Error(stream.str());
      }
      try {
        boost::archive::text_iarchive ia(ifs);
        ia >> (*map);
      }
      catch (boost::archive::archive_exception& e) {
        throw Error(e.what());
      }
    }
    return map;
  }

  case 3: {
    Permutation *permutation = new Permutation;
    Translation *translation = new Translation;
    if (options.with_map_random()) {
      permutation->random(options.get_bv_size());
      translation->random(options.get_bv_size());
    } else {
      std::ifstream ifs(options.get_map_path());
      if (!ifs.good()) {
        std::ostringstream stream;
        stream << "make_map (Composition of permutation and translation): Cannot open " << options.get_map_path();
        throw Error(stream.str());
      }
      try {
        boost::archive::text_iarchive ia(ifs);
        ia >> (*permutation) >> (*translation);
      }
      catch (boost::archive::archive_exception& e) {
        throw Error(e.what());
      }
    }
    return new MapComposition(permutation, translation);
  }

  case 4: {
    LinearMap *map = new LinearMap;
    if (options.with_map_random()) {
      map->random(options.get_bv_size(), options.get_map_input_size(), options.with_map_surjective());
    } else {
      std::ifstream ifs(options.get_map_path());
      if (!ifs.good()) {
        std::ostringstream stream;
        stream << "make_map (LinearMap): Cannot open " << options.get_map_path();
        throw Error(stream.str());
      }
      try {
        boost::archive::text_iarchive ia(ifs);
        ia >> (*map);
      }
      catch (boost::archive::archive_exception& e) {
        throw Error(e.what());
      }
    }
    return map;
  }

  case 5: {
    AffineMap *map = new AffineMap;
    if (options.with_map_random()) {
      map->random(options.get_bv_size(), options.get_map_input_size(), options.with_map_surjective());
    } else {
      std::ifstream ifs(options.get_map_path());
      if (!ifs.good()) {
        std::ostringstream stream;
        stream << "make_map (AffineMap): Cannot open " << options.get_map_path();
        throw Error(stream.str());
      }
      try {
        boost::archive::text_iarchive ia(ifs);
        ia >> (*map);
      }
      catch (boost::archive::archive_exception& e) {
        throw Error(e.what());
      }
    }
    return map;
  }

  default: {
    std::ostringstream stream;
    stream << "make_map: Unknown map type: " << options.get_map();
    throw Error(stream.str());
  }

  }

}


Neighborhood *
make_prior_noise_neighborhood(const Options& options)
{
  switch(options.get_pn_neighborhood()) {

  case 0:
    return new SingleBitFlip
      (options.get_bv_size());

  case 1: {
    auto nh = new BernoulliProcess
      (options.get_bv_size(),
       options.get_pn_mutation_probability() / options.get_bv_size());
    nh->set_allow_stay(options.with_pn_allow_stay());
    return nh;
  }

  case 2:
    return new HammingBall
      (options.get_bv_size(),
       options.get_pn_radius());

  case 3:
    return new HammingSphere
      (options.get_bv_size(),
       options.get_pn_radius());

  default:
    std::ostringstream stream;
    stream << "make_prior_noise_neighborhood: Unknown neighborhood type: " << options.get_neighborhood();
    throw Error(stream.str());
  }

}


Function *
make_function_modifier(Function *function, const Options& options)
{
  assert(function);

  const size_t bv_size = function->get_bv_size();

  // Map
  Map *map = 0;
  if (options.get_map() > 0) {
    map = make_map(options);
    function = new FunctionMapComposition(function, map);
  }

  if (function->get_bv_size() != bv_size) {
    std::cerr << "Warning: make_function_modifier: After composition by a map, bv_size changed from "
              << bv_size << " to "
              << function->get_bv_size() << std::endl;
  }

  // Prior noise
  if (options.with_prior_noise()) {
    Neighborhood *nh = make_prior_noise_neighborhood(options);
    function = new PriorNoise(function, nh);
  }

  // Negation
  if (options.with_negation()) {
    function = new Negation(function);
  }

  // Additive gaussian noise
  if (options.with_additive_gaussian_noise()) {
    function = new AdditiveGaussianNoise(function, options.get_noise_stddev());
  }

  return function;
}


Function *
make_function_controller(Function *function, const Options& options)
{
  assert(function);

  // Stop on maximum
  if (options.with_stop_on_maximum()) {

    // Known maximum
    if (function->has_known_maximum()) {
      function = new StopOnMaximum(function);
    } else {
      std::ostringstream stream;
      stream << "make_function_controller (StopOnMaximum): Unknown maximum";
      throw Error(stream.str());
    }

  }

  // Stop on target
  if (options.with_stop_on_target()) {
    function = new StopOnTarget(function, options.get_target());
  }

  // Budget
  if (options.get_budget() > 0) {
    function = new OnBudgetFunction(function, options.get_budget());
  }

  // Cache
  if (options.with_cache()) {
    function = new Cache(function);
  }

  // Cache budget
  if (options.get_cache_budget() > 0) {
    function = new OnBudgetFunction(function, options.get_budget());
  }

  return function;
}


Function *
make_function(Options& options)
{
  const int bv_size = options.get_bv_size();

  Function *function = make_concrete_function(options);
  assert(function);

  if (int(function->get_bv_size()) != bv_size) {
    std::cerr << "Warning: make_function: After make_concrete_function, bv_size changed from "
              << bv_size << " to "
              << function->get_bv_size() << std::endl;
  }

  function = make_function_modifier(function, options);
  function = make_function_controller(function, options);
  assert(function);

  options.set_bv_size(function->get_bv_size());
  if (options.get_bv_size() <= 0)
    throw Error("make_function: bv_size must be positive");

  return function;
}
