/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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
using namespace hnco::function::controller;
using namespace hnco::function::modifier;
using namespace hnco::neighborhood;
using namespace hnco;


template<class T>
void load_function_from_boost_archive(T *fn, const Options& options)
{
  std::ifstream ifs(options.get_path());
  if (!ifs.good()) {
    std::ostringstream stream;
    stream << "load_function_from_boost_archive: Cannot open " << options.get_path();
    throw Error(stream.str());
  }
  try {
    boost::archive::text_iarchive archive(ifs);
    archive >> (*fn);
  }
  catch (boost::archive::archive_exception& e) {
    throw Error(e.what());
  }
}


template<class T>
void load_function_from_file(T *fn, const Options& options)
{
  std::ifstream ifs(options.get_path());
  if (!ifs.good()) {
    std::ostringstream stream;
    stream << "load_function_from_file: Cannot open " << options.get_path();
    throw Error(stream.str());
  }
  fn->load(ifs);
}


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
    load_function_from_boost_archive<LinearFunction>(function, options);
    return function;
  }

  case 5: {
    LinearFunction* function = new LinearFunction;
    function->generate(options.get_bv_size(), [w = 1] () mutable { double result = w; w *= 2; return result; });
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
    Qubo* function = new Qubo;
    load_function_from_file<Qubo>(function, options);
    return function;
  }

  case 60: {
    NkLandscape* function = new NkLandscape;
    load_function_from_boost_archive<NkLandscape>(function, options);
    return function;
  }

  case 70: {
    MaxSat *function = new MaxSat;
    load_function_from_file<MaxSat>(function, options);
    return function;
  }

  case 71: {
    MaxNae3Sat *function = new MaxNae3Sat;
    load_function_from_file<MaxNae3Sat>(function, options);
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
    load_function_from_boost_archive<EqualProducts>(function, options);
    return function;
  }

  case 91: {
    Partition* function = new Partition;
    load_function_from_boost_archive<Partition>(function, options);
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
  case 150: {
    Factorization *function = new Factorization;
    load_function_from_file<Factorization>(function, options);
    return function;
  }
#endif

  case 160: {
    WalshExpansion* function = new WalshExpansion;
    load_function_from_boost_archive<WalshExpansion>(function, options);
    return function;
  }

  case 161: {
    WalshExpansion1* function = new WalshExpansion1;
    load_function_from_boost_archive<WalshExpansion1>(function, options);
    return function;
  }

  case 162: {
    WalshExpansion2* function = new WalshExpansion2;
    load_function_from_boost_archive<WalshExpansion2>(function, options);
    return function;
  }

  case 171: {
    NearestNeighborIsingModel1* function = new NearestNeighborIsingModel1;
    load_function_from_boost_archive<NearestNeighborIsingModel1>(function, options);
    return function;
  }

  case 172: {
    NearestNeighborIsingModel2* function = new NearestNeighborIsingModel2;
    load_function_from_boost_archive<NearestNeighborIsingModel2>(function, options);
    return function;
  }

  case 180: {
    using namespace hnco::function::representation;
    using Rep = DyadicRealRepresentation<double>;
    using Fn = ParsedMultivariateFunction<FunctionParser, Rep::value_type>;
    auto fn = new Fn(options.get_fp_expression());
    auto reps = std::vector<Rep>
      (fn->get_num_variables(),
       Rep(options.get_fp_num_bits(),
           options.get_fp_lower_bound(),
           options.get_fp_upper_bound()));
    return new MultivariateFunctionAdapter<Fn, Rep>(fn, reps);
  }

  case 181: {
    using namespace hnco::function::representation;
    using Rep = DyadicIntegerRepresentation<long>;
    using Fn = ParsedMultivariateFunction<FunctionParser_li, Rep::value_type>;
    auto fn = new Fn(options.get_fp_expression());
    auto reps = std::vector<Rep>
      (fn->get_num_variables(),
       Rep(options.get_fp_num_bits(),
           options.get_fp_lower_bound(),
           options.get_fp_upper_bound()));
    return new MultivariateFunctionAdapter<Fn, Rep>(fn, reps);
  }

  case 182: {
    using namespace hnco::function::representation;
    using Rep = DyadicComplexRepresentation<double>;
    using Fn = ParsedMultivariateFunction<FunctionParser_cd, Rep::value_type>;
    auto fn = new Fn(options.get_fp_expression());
    auto reps = std::vector<Rep>
      (fn->get_num_variables(),
       Rep(options.get_fp_num_bits(),
           options.get_fp_lower_bound(),
           options.get_fp_upper_bound()));
    return new MultivariateFunctionAdapter<Fn, Rep>(fn, reps);
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


template<class T>
void load_map(T *map, const Options& options)
{
  std::ifstream ifs(options.get_map_path());
  if (!ifs.good()) {
    std::ostringstream stream;
    stream << "load_map: Cannot open " << options.get_map_path();
    throw Error(stream.str());
  }
  try {
    boost::archive::text_iarchive archive(ifs);
    archive >> (*map);
  }
  catch (boost::archive::archive_exception& e) {
    throw Error(e.what());
  }
}


Map *
make_map(const Options& options)
{
  assert(options.get_map() > 0);
  assert(options.get_bv_size() > 0);

  switch(options.get_map()) {

  case 1: {
    Translation *map = new Translation;
    if (options.with_map_random()) {
      map->random(options.get_bv_size());
    } else
      load_map<Translation>(map, options);
    return map;
  }

  case 2: {
    Permutation *map = new Permutation;
    if (options.with_map_random()) {
      map->random(options.get_bv_size());
    } else
      load_map<Permutation>(map, options);
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
      if (options.get_map_input_size() <= 0)
        throw Error("make_map: map_input_size must be positive");
      map->random(options.get_bv_size(), options.get_map_input_size(), options.with_map_surjective());
    } else
      load_map<LinearMap>(map, options);
    return map;
  }

  case 5: {
    AffineMap *map = new AffineMap;
    if (options.with_map_random()) {
      if (options.get_map_input_size() <= 0)
        throw Error("make_map: map_input_size must be positive");
      map->random(options.get_bv_size(), options.get_map_input_size(), options.with_map_surjective());
    } else
      load_map<AffineMap>(map, options);
    return map;
  }

  case 6: {
    TsAffineMap *map = new TsAffineMap;
    if (options.with_map_random()) {
      if (options.get_map_input_size() != options.get_bv_size())
        std::cerr << "Warning: make_map: TsAffineMap: using bv_size, discarding map_input_size" << std::endl;
      map->random(options.get_bv_size(),
                  options.get_map_ts_length(),
                  static_cast<TsAffineMap::SamplingMode>(options.get_map_ts_sampling_mode()));
    } else
      load_map<TsAffineMap>(map, options);
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
FunctionFactory::make_function_modifier(Function *function, Options& options)
{
  assert(function);

  // Map
  if (options.get_map() > 0) {
    _map = make_map(options);
    function = new FunctionMapComposition(function, _map);
  }

  if (int(function->get_bv_size()) != options.get_bv_size()) {
    std::cerr << "Warning: make_function_modifier: After composition by a map, bv_size changed from "
              << options.get_bv_size() << " to "
              << function->get_bv_size() << std::endl;
    options.set_bv_size(function->get_bv_size());
  }

  // Prior noise
  if (options.with_prior_noise()) {
    Neighborhood *nh = make_prior_noise_neighborhood(options);
    function = new PriorNoise(function, nh);
  }

  // Expression modifier
  if (options.with_parsed_modifier()) {
    function = new ParsedModifier(function, options.get_expression());
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
FunctionFactory::make_function_controller(Function *function, const Options& options)
{
  assert(function);

  _tracker = new ProgressTracker(function);
  _tracker->set_log_improvement(options.with_log_improvement());
  _tracker->set_record_evaluation_time(options.with_record_evaluation_time());

  function = _tracker;

  //
  // The following conditions are mutually exclusive
  //

  if (options.get_budget() > 0 && !options.with_cache()) {
    function = new OnBudgetFunction(function, options.get_budget());
  }

  if (options.get_budget() <= 0 && options.with_cache()) {
    _cache = new Cache(function);
    function = _cache;
  }

  if (options.get_budget() > 0 && options.with_cache()) {
    if (options.with_cache_budget()) {
      _cache = new Cache(function);
      function = new OnBudgetFunction(_cache, options.get_budget());
    } else {
      function = new OnBudgetFunction(function, options.get_budget());
      _cache = new Cache(function);
      function = _cache;
    }
  }

  // Stop on maximum
  if (options.with_stop_on_maximum()) {

    // Requires known maximum
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

  return function;
}


Function *
FunctionFactory::make_function(Options& options)
{
  if (options.get_bv_size() <= 0)
    throw Error("make_function: bv_size must be positive");

  Function *function = make_concrete_function(options);

  if (int(function->get_bv_size()) != options.get_bv_size()) {
    std::cerr << "Warning: make_function: After make_concrete_function, bv_size changed from "
              << options.get_bv_size() << " to "
              << function->get_bv_size() << std::endl;
    options.set_bv_size(function->get_bv_size());
  }

  assert(options.get_bv_size() > 0);

  function = make_function_modifier(function, options);

  return function;
}
