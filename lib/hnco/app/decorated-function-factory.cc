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

#include <fstream>              // std::ifstream

#include "hnco/neighborhoods/neighborhood.hh"

#include "decorated-function-factory.hh"

using namespace hnco::algorithm;
using namespace hnco::app;
using namespace hnco::exception;
using namespace hnco::function::controller;
using namespace hnco::function::modifier;
using namespace hnco::function;
using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;


template<class T>
void load_map(T *map, const HncoOptions& options)
{
  std::ifstream ifs(options.get_map_path());
  if (!ifs.good()) {
    throw std::runtime_error("load_map: Cannot open " + options.get_map_path());
  }
  try {
    boost::archive::text_iarchive archive(ifs);
    archive >> (*map);
  }
  catch (boost::archive::archive_exception& e) {
    throw std::runtime_error(e.what());
  }
}


Map *
make_map(const HncoOptions& options, int bv_size)
{
  assert(options.get_map() > 0);
  assert(bv_size > 0);

  switch(options.get_map()) {

  case 1: {
    Translation *map = new Translation;
    if (options.with_map_random()) {
      if (options.get_map_input_size() != bv_size)
        std::cerr << "Warning: make_map: Translation: using bv_size, discarding map_input_size" << std::endl;
      map->random(bv_size);
    } else
      load_map<Translation>(map, options);
    return map;
  }

  case 2: {
    Permutation *map = new Permutation;
    if (options.with_map_random()) {
      if (options.get_map_input_size() != bv_size)
        std::cerr << "Warning: make_map: Permutation: using bv_size, discarding map_input_size" << std::endl;
      map->random(bv_size);
    } else
      load_map<Permutation>(map, options);
    return map;
  }

  case 3: {
    Permutation *permutation = new Permutation;
    Translation *translation = new Translation;
    if (options.with_map_random()) {
      if (options.get_map_input_size() != bv_size)
        std::cerr << "Warning: make_map: Composition of permutation and translation: using bv_size, discarding map_input_size" << std::endl;
      permutation->random(bv_size);
      translation->random(bv_size);
    } else {
      std::ifstream ifs(options.get_map_path());
      if (!ifs.good()) {
        throw std::runtime_error("make_map (Composition of permutation and translation): Cannot open " + options.get_map_path());
      }
      try {
        boost::archive::text_iarchive ia(ifs);
        ia >> (*permutation) >> (*translation);
      }
      catch (boost::archive::archive_exception& e) {
        throw std::runtime_error(e.what());
      }
    }
    return new MapComposition(permutation, translation);
  }

  case 4: {
    LinearMap *map = new LinearMap;
    if (options.with_map_random()) {
      if (options.get_map_input_size() <= 0)
        throw std::runtime_error("make_map: map_input_size must be positive");
      map->random(bv_size, options.get_map_input_size(), options.with_map_surjective());
    } else
      load_map<LinearMap>(map, options);
    return map;
  }

  case 5: {
    AffineMap *map = new AffineMap;
    if (options.with_map_random()) {
      if (options.get_map_input_size() <= 0)
        throw std::runtime_error("make_map: map_input_size must be positive");
      map->random(bv_size, options.get_map_input_size(), options.with_map_surjective());
    } else
      load_map<AffineMap>(map, options);
    return map;
  }

  case 6: {
    TsAffineMap *map = new TsAffineMap;
    if (options.with_map_random()) {
      if (options.get_map_input_size() != bv_size)
        std::cerr << "Warning: make_map: TsAffineMap: using bv_size, discarding map_input_size" << std::endl;
      map->random(bv_size,
                  options.get_map_ts_length(),
                  static_cast<TsAffineMap::SamplingMode>(options.get_map_ts_sampling_mode()));
    } else
      load_map<TsAffineMap>(map, options);
    return map;
  }

  default: {
    throw std::runtime_error("make_map: Unknown map type: " + options.get_map());
  }

  }

}


Neighborhood *
make_prior_noise_neighborhood(const HncoOptions& options, int bv_size)
{
  switch(options.get_pn_neighborhood()) {

  case 0:
    return new SingleBitFlip
      (bv_size);

  case 1: {
    auto nh = new StandardBitMutation
      (bv_size,
       options.get_pn_mutation_rate() / bv_size);
    nh->set_allow_no_mutation(options.with_pn_allow_no_mutation());
    return nh;
  }

  case 2:
    return new HammingBall
      (bv_size,
       options.get_pn_radius());

  case 3:
    return new HammingSphere
      (bv_size,
       options.get_pn_radius());

  default:
    throw std::runtime_error("make_prior_noise_neighborhood: Unknown neighborhood type: " + options.get_neighborhood());
  }

}


Function *
DecoratedFunctionFactory::make_function()
{
  if (_options.get_bv_size() <= 0)
    throw std::runtime_error("DecoratedFunctionFactory::make_function: bv_size must be positive");

  Function *function = _function_factory.make();

  if (function->get_bv_size() != _options.get_bv_size()) {
    std::cerr << "Warning: DecoratedFunctionFactory::make_function: After _function_factory.make(), bv_size changed from "
              << _options.get_bv_size() << " to "
              << function->get_bv_size() << std::endl;
  }

  assert(function->get_bv_size() > 0);

  return function;
}


Function *
DecoratedFunctionFactory::make_function_modifier()
{
  Function *function = make_function();
  assert(function);

  int bv_size = function->get_bv_size();

  // Map
  if (_options.get_map() > 0) {
    _map = make_map(_options, bv_size);
    function = new FunctionMapComposition(function, _map);
  }

  if (function->get_bv_size() != bv_size) {
    std::cerr << "Warning: DecoratedFunctionFactory::make_function_modifier: After composition by a map, bv_size changed from "
              << bv_size << " to "
              << function->get_bv_size() << std::endl;
  }

  // Prior noise
  if (_options.with_prior_noise()) {
    Neighborhood *nh = make_prior_noise_neighborhood(_options, bv_size);
    function = new PriorNoise(function, nh);
  }

  // Expression modifier
  if (_options.with_parsed_modifier()) {
    function = new ParsedModifier(function, _options.get_expression());
  }

  // Negation
  if (_options.with_negation()) {
    function = new Negation(function);
  }

  // Additive gaussian noise
  if (_options.with_additive_gaussian_noise()) {
    function = new AdditiveGaussianNoise(function, _options.get_noise_stddev());
  }

  return function;
}


Function *
DecoratedFunctionFactory::make_function_controller(Function *function)
{
  assert(function);

  _tracker = new ProgressTracker(function);
  _tracker->set_log_improvement(_options.with_log_improvement());
  _tracker->set_record_evaluation_time(_options.with_record_evaluation_time());

  function = _tracker;

  //
  // The following conditions are mutually exclusive
  //

  if (_options.get_budget() > 0 && !_options.with_cache()) {
    function = new OnBudgetFunction(function, _options.get_budget());
  }

  if (_options.get_budget() <= 0 && _options.with_cache()) {
    _cache = new Cache(function);
    function = _cache;
  }

  if (_options.get_budget() > 0 && _options.with_cache()) {
    if (_options.with_cache_budget()) {
      _cache = new Cache(function);
      function = new OnBudgetFunction(_cache, _options.get_budget());
    } else {
      function = new OnBudgetFunction(function, _options.get_budget());
      _cache = new Cache(function);
      function = _cache;
    }
  }

  //
  // Stop on target
  //

  if (_options.with_stop_on_maximum()) {
    // Requires known maximum
    if (function->has_known_maximum()) {
      _stop_on_target = new StopOnMaximum(function);
      function = _stop_on_target;
    } else {
      throw std::runtime_error("DecoratedFunctionFactory::make_function_controller (StopOnMaximum): Unknown maximum");
    }
  } else {
    if (_options.with_stop_on_target()) {
      _stop_on_target = new StopOnTarget(function, _options.get_target());
      function = _stop_on_target;
    }
  }

  return function;
}
