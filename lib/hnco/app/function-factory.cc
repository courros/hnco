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
#include <fstream>              // std::ifstream, std::ofstream

#include "config.h"

#include "hnco/functions/all.hh"

#include "function-factory.hh"

using namespace hnco::app;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco;


template<class T>
void load_function_from_boost_archive(T *fn, const HncoOptions& options)
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
void load_function_from_file(T *fn, const HncoOptions& options)
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
CommandLineFunctionFactory::make()
{
  if (_options.get_bv_size() <= 0)
    throw Error("CommandLineFunctionFactory::make: bv_size must be positive");

  switch(_options.get_function()) {

  case 0:
    return new OneMax
      (_options.get_bv_size());

  case 1: {
    LinearFunction* function = new LinearFunction;
    load_function_from_boost_archive<LinearFunction>(function, _options);
    return function;
  }

  case 5: {
    LinearFunction* function = new LinearFunction;
    function->generate(_options.get_bv_size(), [w = 1] () mutable { double result = w; w *= 2; return result; });
    return function;
  }

  case 10:
    return new LeadingOnes
      (_options.get_bv_size());

  case 11:
    return new Ridge
      (_options.get_bv_size());

  case 20:
    return new Needle
      (_options.get_bv_size());

  case 30:
    return new Jump
      (_options.get_bv_size(),
       _options.get_fn_threshold());

  case 31:
    return new DeceptiveJump
      (_options.get_bv_size(),
       _options.get_fn_threshold());

  case 40:
    return new FourPeaks
      (_options.get_bv_size(),
       _options.get_fn_threshold());

  case 41:
    return new SixPeaks
      (_options.get_bv_size(),
       _options.get_fn_threshold());

  case 50: {
    Qubo* function = new Qubo;
    load_function_from_file<Qubo>(function, _options);
    return function;
  }

  case 60: {
    NkLandscape* function = new NkLandscape;
    load_function_from_boost_archive<NkLandscape>(function, _options);
    return function;
  }

  case 70: {
    MaxSat *function = new MaxSat;
    load_function_from_file<MaxSat>(function, _options);
    return function;
  }

  case 71: {
    MaxNae3Sat *function = new MaxNae3Sat;
    load_function_from_file<MaxNae3Sat>(function, _options);
    return function;
  }

  case 80:
    return new Labs
      (_options.get_bv_size());

  case 81:
    return new LabsMeritFactor
      (_options.get_bv_size());

  case 90: {
    EqualProducts* function = new EqualProducts;
    load_function_from_boost_archive<EqualProducts>(function, _options);
    return function;
  }

  case 91: {
    Partition* function = new Partition;
    load_function_from_boost_archive<Partition>(function, _options);
    return function;
  }

  case 100:
    return new SummationCancellation
      (_options.get_bv_size());

  case 101:
    return new SinusSummationCancellation
      (_options.get_bv_size());

  case 110:
    return new Trap
      (_options.get_bv_size(),
       _options.get_fn_num_traps());

  case 120:
    return new Hiff
      (_options.get_bv_size());

  case 130:
    return new Plateau
      (_options.get_bv_size());

  case 140:
    return new LongPath
      (_options.get_bv_size(),
       _options.get_fn_prefix_length());

#ifdef ENABLE_FACTORIZATION
  case 150: {
    Factorization *function = new Factorization;
    load_function_from_file<Factorization>(function, _options);
    return function;
  }
#endif

  case 160: {
    WalshExpansion* function = new WalshExpansion;
    load_function_from_boost_archive<WalshExpansion>(function, _options);
    return function;
  }

  case 161: {
    WalshExpansion1* function = new WalshExpansion1;
    load_function_from_boost_archive<WalshExpansion1>(function, _options);
    return function;
  }

  case 162: {
    WalshExpansion2* function = new WalshExpansion2;
    load_function_from_boost_archive<WalshExpansion2>(function, _options);
    return function;
  }

  case 171: {
    NearestNeighborIsingModel1* function = new NearestNeighborIsingModel1;
    load_function_from_boost_archive<NearestNeighborIsingModel1>(function, _options);
    return function;
  }

  case 172: {
    NearestNeighborIsingModel2* function = new NearestNeighborIsingModel2;
    load_function_from_boost_archive<NearestNeighborIsingModel2>(function, _options);
    return function;
  }

  case 180: {
    using namespace hnco::function::representation;
    using Rep = DyadicRealRepresentation<double>;
    using Fn = ParsedMultivariateFunction<FunctionParser>;
    using Conv = ScalarToDouble<double>;
    auto fn = new Fn(_options.get_fp_expression());
    auto reps = std::vector<Rep>
      (fn->get_num_variables(),
       Rep(_options.get_fp_num_bits(),
           _options.get_fp_lower_bound(),
           _options.get_fp_upper_bound()));
    return new MultivariateFunctionAdapter<Fn, Rep, Conv>(fn, reps);
  }

  case 181: {
    using namespace hnco::function::representation;
    using Rep = DyadicIntegerRepresentation<long>;
    using Fn = ParsedMultivariateFunction<FunctionParser_li>;
    using Conv = ScalarToDouble<long>;
    auto fn = new Fn(_options.get_fp_expression());
    auto reps = std::vector<Rep>
      (fn->get_num_variables(),
       Rep(_options.get_fp_num_bits(),
           _options.get_fp_lower_bound(),
           _options.get_fp_upper_bound()));
    return new MultivariateFunctionAdapter<Fn, Rep, Conv>(fn, reps);
  }

  case 182: {
    using namespace hnco::function::representation;
    using Rep = DyadicComplexRepresentation<double>;
    using Fn = ParsedMultivariateFunction<FunctionParser_cd>;
    using Conv = ComplexToDouble<double>;
    auto fn = new Fn(_options.get_fp_expression());
    auto reps = std::vector<Rep>
      (fn->get_num_variables(),
       Rep(_options.get_fp_num_bits(),
           _options.get_fp_lower_bound(),
           _options.get_fp_upper_bound()));
    return new MultivariateFunctionAdapter<Fn, Rep, Conv>(fn, reps);
  }

  case 190: {
    using namespace hnco::function::representation;
    using Fn = Sudoku;
    using Conv = ScalarToDouble<double>;
    auto fn = new Fn();
    load_function_from_file<Sudoku>(fn, _options);
    switch (_options.get_rep_categorical_representation()) {
    case 0: {
      using Rep = IntegerCategoricalRepresentation;
      auto reps = std::vector<Rep>(fn->get_num_variables(), Rep(9));
      return new MultivariateFunctionAdapter<Fn, Rep, Conv>(fn, reps);
    }
    case 1: {
      using Rep = LinearCategoricalRepresentation;
      auto reps = std::vector<Rep>(fn->get_num_variables(), Rep(9));
      return new MultivariateFunctionAdapter<Fn, Rep, Conv>(fn, reps);
    }
    default:
      std::ostringstream stream;
      stream << "CommandLineFunctionFactory::make: Unknown categorical representation type: " << _options.get_rep_categorical_representation();
      throw Error(stream.str());
    }
  }

#ifdef ENABLE_PLUGIN
  case 1000:
    return new FunctionPlugin
      (_options.get_bv_size(),
       _options.get_path(),
       _options.get_fn_name());
#endif

  default:
    std::ostringstream stream;
    stream << "CommandLineFunctionFactory::make: Unknown function type: " << _options.get_function();
    throw Error(stream.str());
  }

}
