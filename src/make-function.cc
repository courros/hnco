/* Copyright (C) 2016, 2017 Arnaud Berny

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
#include "hnco/neighborhoods/neighborhood.hh"

#include "make-function.hh"

using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::neighborhood;
using namespace hnco;


Function *
make_concrete_function(Options& options)
{
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
    boost::archive::text_iarchive ia(ifs);
    ia >> (*function);
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
       options.get_fun_threshold());

  case 31:
    return new DeceptiveJump
      (options.get_bv_size(),
       options.get_fun_threshold());

  case 40:
    return new FourPeaks
      (options.get_bv_size(),
       options.get_fun_threshold());

  case 41:
    return new SixPeaks
      (options.get_bv_size(),
       options.get_fun_threshold());

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
    boost::archive::text_iarchive ia(ifs);
    ia >> (*function);
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

  case 90: {
    EqualProducts* function = new EqualProducts;
    std::ifstream ifs(options.get_path());
    if (!ifs.good()) {
      std::ostringstream stream;
      stream << "make_concrete_function (EqualProducts): Cannot open " << options.get_path();
      throw Error(stream.str());
    }
    boost::archive::text_iarchive ia(ifs);
    ia >> (*function);
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
       options.get_fun_num_traps());

  case 120:
    return new Hiff
      (options.get_bv_size());

  case 130:
    return new Plateau
      (options.get_bv_size());

  case 140:
    return new LongPath
      (options.get_bv_size(),
       options.get_fun_prefix_length());

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
    boost::archive::text_iarchive ia(ifs);
    ia >> (*function);
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
    boost::archive::text_iarchive ia(ifs);
    ia >> (*function);
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
    boost::archive::text_iarchive ia(ifs);
    ia >> (*function);
    return function;
  }

  case 1000:
    return new FunctionPlugin
      (options.get_bv_size(),
       options.get_path(),
       options.get_fun_name());

  default:
    std::ostringstream stream;
    stream << "make_concrete_function: Unknown function type: " << options.get_function();
    throw Error(stream.str());
  }

}


Map *
make_map(Options& options)
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
        stream << "make_concrete_function (Translation): Cannot open " << options.get_map_path();
        throw Error(stream.str());
      }
      boost::archive::text_iarchive ia(ifs);
      ia >> (*map);
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
        stream << "make_concrete_function (Permutation): Cannot open " << options.get_map_path();
        throw Error(stream.str());
      }
      boost::archive::text_iarchive ia(ifs);
      ia >> (*map);
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
        stream << "make_concrete_function (Composition of permutation and translation): Cannot open " << options.get_map_path();
        throw Error(stream.str());
      }
      boost::archive::text_iarchive ia(ifs);
      ia >> (*permutation) >> (*translation);
    }
    return new MapComposition(permutation, translation);
  }

  case 4: {
    LinearMap *map = new LinearMap;
    if (options.with_map_random()) {
      map->random(options.get_bv_size(), options.get_map_input_size());
    } else {
      std::ifstream ifs(options.get_map_path());
      if (!ifs.good()) {
        std::ostringstream stream;
        stream << "make_concrete_function (LinearMap): Cannot open " << options.get_map_path();
        throw Error(stream.str());
      }
      boost::archive::text_iarchive ia(ifs);
      ia >> (*map);
    }
    return map;
  }

  case 5: {
    AffineMap *map = new AffineMap;
    if (options.with_map_random()) {
      map->random(options.get_bv_size(), options.get_map_input_size());
    } else {
      std::ifstream ifs(options.get_map_path());
      if (!ifs.good()) {
        std::ostringstream stream;
        stream << "make_concrete_function (AffineMap): Cannot open " << options.get_map_path();
        throw Error(stream.str());
      }
      boost::archive::text_iarchive ia(ifs);
      ia >> (*map);
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
make_prior_noise_neighborhood(Options& options)
{
  switch(options.get_pn_neighborhood()) {

  case 0:
    return new SingleBitFlip
      (options.get_bv_size());

  case 1: {
    auto nh = new BernoulliProcess
      (options.get_bv_size(),
       options.get_pn_mutation() / options.get_bv_size());
    nh->_allow_stay = options.with_pn_allow_stay();
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
    stream << options.get_neighborhood();
    throw Error("make_prior_noise_neighborhood: Unknown neighborhood type: " + stream.str());
  }
}


Function *
make_function_decorator(Function *function, Options& options)
{
  assert(function);
  assert(options.get_bv_size() > 0);

  // Map
  Map *map = 0;
  if (options.get_map() > 0) {
    map = make_map(options);
    assert(map);
    function = new FunctionMapComposition(function, map);
    assert(function);
  }

  int bv_size = options.get_bv_size();

  if (bv_size != int(function->get_bv_size())) {
    std::cerr << "Warning: After make_map, bv_size changed from " << bv_size << " to " << function->get_bv_size() << std::endl;
    bv_size = function->get_bv_size();
    options.set_bv_size(function->get_bv_size());
  }

  assert(bv_size == options.get_bv_size());

  // Additive gaussian noise
  if (options.with_additive_gaussian_noise()) {
    function = new AdditiveGaussianNoise(function, options.get_noise_stddev());
    assert(function);
  }

  // Negation
  if (options.with_negation()) {
    function = new Negation(function);
    assert(function);
  }

  // Stop on maximum
  if (options.with_stop_on_maximum()) {

    // Bijective map
    if (options.get_map() > 0) {
      assert(map);
      if (!map->is_surjective())
        throw Error("make_function_decorator: StopOnMaximum requires a bijective map");
    }

    // Known maximum
    if (function->has_known_maximum()) {
      function = new StopOnMaximum(function);
      assert(function);
    } else {
      std::ostringstream stream;
      stream << "make_function_decorator: Function " << options.get_function() << ": Unknown maximum";
      throw Error(stream.str());
    }

  }

  // Stop on target
  if (options.with_stop_on_target()) {
    function = new StopOnTarget(function, options.get_target());
    assert(function);
  }

  // Cache
  if (options.with_cache()) {
    function = new Cache(function);
    assert(function);
  }

  // Budget
  if (options.get_budget() > 0) {
    function = new OnBudgetFunction(function, options.get_budget());
    assert(function);
  }

  assert(options.get_bv_size() == bv_size);

  return function;
}


Function *
make_function(Options& options)
{
  assert(options.get_bv_size() > 0);

  Function *function = make_concrete_function(options);
  assert(function);

  int bv_size = options.get_bv_size();

  if (bv_size != int(function->get_bv_size())) {
    std::cerr << "Warning: After make_concrete_function, bv_size changed from " << bv_size << " to " << function->get_bv_size() << std::endl;
    bv_size = function->get_bv_size();
    options.set_bv_size(function->get_bv_size());
  }

  assert(bv_size == options.get_bv_size());

  function = make_function_decorator(function, options);
  assert(function);

  return function;
}
