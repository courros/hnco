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
#include <string>
#include <sstream>
#include <unordered_map>
#include <optional>

#include "config.h"

#include "hnco/functions/all.hh"

#include "function-factory.hh"

using namespace hnco::app;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco;


template<typename T>
using Interval = std::pair<T, T>;

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Interval<T> interval)
{
  stream << "[" << interval.first << ", " << interval.second << "]";
  return stream;
}

template<typename T>
std::optional<Interval<T>> parse_interval(std::istringstream &stream)
{
  char c;
  stream >> c;
  if (stream.fail() || c != '[') {
    std::cerr << "parse_interval: Expected [" << std::endl;
    return {};
  }

  Interval<T> interval;

  stream >> interval.first;
  if (stream.fail()) {
    std::cerr << "parse_interval: Expected lower bound" << std::endl;
    return {};
  }

  stream >> c;
  if (stream.fail() || c != ',') {
    std::cerr << "parse_interval: Expected ," << std::endl;
    return {};
  }

  stream >> interval.second;
  if (stream.fail()) {
    std::cerr << "parse_interval: Expected upper bound" << std::endl;
    return {};
  }

  stream >> c;
  if (stream.fail() || c != ']') {
    std::cerr << "parse_interval: Expected ]" << std::endl;
    return {};
  }

  return interval;
}

template<typename T>
std::unordered_map<std::string, Interval<T>> parse_intervals(std::string str)
{
  std::unordered_map<std::string, Interval<T>> intervals;
  std::istringstream stream(str);

  while (stream.peek() != EOF) {
    std::string name;
    stream >> name;
    if (stream.fail()) {
      std::cerr << "parse_intervals: Expected variable name" << std::endl;
      break;
    }
    std::string keyword;
    stream >> keyword;
    if (stream.fail() || keyword != "in") {
      std::cerr << "parse_intervals: Expected keyword in followed by space after " << name << std::endl;
      break;
    }
    auto opt = parse_interval<T>(stream);
    if (opt)
      intervals[name] = opt.value();
    else
      std::cerr << "parse_intervals: Failed to parse interval for variable " << name << std::endl;
  };

  return intervals;
}

Function *
CommandLineFunctionFactory::make()
{
  if (_options.get_bv_size() <= 0)
    throw std::runtime_error("CommandLineFunctionFactory::make: bv_size must be positive");

  switch(_options.get_function()) {

  case 0:
    return new OneMax
      (_options.get_bv_size());

  case 1: {
    LinearFunction* instance = new LinearFunction;
    instance->load(_options.get_path());
    return instance;
  }

  case 5: {
    LinearFunction* instance = new LinearFunction;
    instance->generate(_options.get_bv_size(), [w = 1] () mutable { double result = w; w *= 2; return result; });
    return instance;
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
    Qubo* instance = new Qubo;
    instance->load(_options.get_path());
    return instance;
  }

  case 60: {
    NkLandscape* instance = new NkLandscape;
    instance->load(_options.get_path());
    return instance;
  }

  case 70: {
    MaxSat *instance = new MaxSat;
    instance->load(_options.get_path());
    return instance;
  }

  case 71: {
    MaxNae3Sat *instance = new MaxNae3Sat;
    instance->load(_options.get_path());
    return instance;
  }

  case 80:
    return new Labs
      (_options.get_bv_size());

  case 81: {
    Labs *instance = new Labs
      (_options.get_bv_size());
    instance->set_merit_factor_flag(true);
    return instance;
  }

  case 90: {
    EqualProducts* instance = new EqualProducts;
    instance->load(_options.get_path());
    return instance;
  }

  case 91: {
    Partition* instance = new Partition;
    instance->load(_options.get_path());
    return instance;
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
    Factorization *instance = new Factorization;
    instance->load(_options.get_path());
    return instance;
  }
#endif

  case 160: {
    WalshExpansion* instance = new WalshExpansion;
    instance->load(_options.get_path());
    return instance;
  }

  case 161: {
    WalshExpansion1* instance = new WalshExpansion1;
    instance->load(_options.get_path());
    return instance;
  }

  case 162: {
    WalshExpansion2* instance = new WalshExpansion2;
    instance->load(_options.get_path());
    return instance;
  }

  case 171: {
    NearestNeighborIsingModel1* instance = new NearestNeighborIsingModel1;
    instance->load(_options.get_path());
    return instance;
  }

  case 172: {
    NearestNeighborIsingModel2* instance = new NearestNeighborIsingModel2;
    instance->load(_options.get_path());
    return instance;
  }

  case 180: {
    using namespace hnco::representation;
    using Rep = DyadicFloatRepresentation<double>;
    using Fn = ParsedMultivariateFunction<FunctionParser>;
    using Conv = ScalarToDouble<double>;
    auto instance = new Fn(_options.get_fp_expression());

    Interval<double> default_interval;
    std::istringstream stream(_options.get_fp_default_interval());
    auto opt = parse_interval<double>(stream);
    if (opt)
      default_interval = opt.value();
    else
      throw std::runtime_error("CommandLineFunctionFactory::make: Function 180: Bad default interval: " + _options.get_fp_default_interval());

    auto intervals = parse_intervals<double>(_options.get_fp_intervals());

    std::vector<Rep> reps;
    for (const auto& name : instance->get_variable_names()) {
      Interval<double> interval;
      if (intervals.count(name)) {
        interval = intervals[name];
      } else {
        interval = default_interval;
        std::cerr << "Warning: CommandLineFunctionFactory::make: No interval for " << name
                  << " hence using default interval " << _options.get_fp_default_interval() << std::endl;
      }
      if (_options.with_fp_precision())
        reps.push_back(Rep(interval.first,
                           interval.second,
                           _options.get_fp_precision()));
      else
        reps.push_back(Rep(interval.first,
                           interval.second,
                           _options.get_fp_num_bits()));
    }

    return new MultivariateFunctionAdapter<Fn, Rep, Conv>(instance, reps);
  }

  case 181: {
    using namespace hnco::representation;
    using Rep = DyadicIntegerRepresentation<long>;
    using Fn = ParsedMultivariateFunction<FunctionParser_li>;
    using Conv = ScalarToDouble<long>;
    auto instance = new Fn(_options.get_fp_expression());

    Interval<long> default_interval;
    std::istringstream stream(_options.get_fp_default_interval());
    auto opt = parse_interval<long>(stream);
    if (opt)
      default_interval = opt.value();
    else
      throw std::runtime_error("CommandLineFunctionFactory::make: Function 181: Bad default interval: " + _options.get_fp_default_interval());

    auto intervals = parse_intervals<long>(_options.get_fp_intervals());

    std::vector<Rep> reps;
    for (const auto& name : instance->get_variable_names()) {
      Interval<long> interval;
      if (intervals.count(name)) {
        interval = intervals[name];
      } else {
        interval = default_interval;
        std::cerr << "Warning: CommandLineFunctionFactory::make: Function 181: No interval for " << name
                  << " hence using default interval " << _options.get_fp_default_interval() << std::endl;
      }
      reps.push_back(Rep(interval.first, interval.second));
    }

    return new MultivariateFunctionAdapter<Fn, Rep, Conv>(instance, reps);
  }

  case 182: {
    using namespace hnco::representation;
    using FloatRep = DyadicFloatRepresentation<double>;
    using Rep = DyadicComplexRepresentation<double>;
    using Fn = ParsedMultivariateFunction<FunctionParser_cd>;
    using Conv = ComplexToDouble<double>;
    auto instance = new Fn(_options.get_fp_expression());

    Interval<double> default_interval;
    std::istringstream stream(_options.get_fp_default_interval());
    auto opt = parse_interval<double>(stream);
    if (opt)
      default_interval = opt.value();
    else
      throw std::runtime_error("CommandLineFunctionFactory::make: Function 182: Bad default interval: " + _options.get_fp_default_interval());

    auto intervals = parse_intervals<double>(_options.get_fp_intervals());

    std::vector<Rep> reps;
    for (const auto& name : instance->get_variable_names()) {
      Interval<double> interval_re, interval_im;
      std::string name_re = name + "_re";
      std::string name_im = name + "_im";
      if (intervals.count(name_re)) {
        interval_re = intervals[name_re];
      } else {
        interval_re = default_interval;
        std::cerr << "Warning: CommandLineFunctionFactory::make: No interval for " << name_re
                  << " hence using default interval " << _options.get_fp_default_interval() << std::endl;
      }
      if (intervals.count(name_im)) {
        interval_im = intervals[name_im];
      } else {
        interval_im = default_interval;
        std::cerr << "Warning: CommandLineFunctionFactory::make: No interval for " << name_im
                  << " hence using default interval " << _options.get_fp_default_interval() << std::endl;
      }
      if (_options.with_fp_precision()) {
        FloatRep rep_re(interval_re.first,
                        interval_re.second,
                        _options.get_fp_precision());
        FloatRep rep_im(interval_im.first,
                        interval_im.second,
                        _options.get_fp_precision());
        reps.push_back(Rep(rep_re, rep_im));
      } else {
        FloatRep rep_re(interval_re.first,
                        interval_re.second,
                        _options.get_fp_num_bits());
        FloatRep rep_im(interval_im.first,
                        interval_im.second,
                        _options.get_fp_num_bits());
        reps.push_back(Rep(rep_re, rep_im));
      }

    }

    return new MultivariateFunctionAdapter<Fn, Rep, Conv>(instance, reps);
  }

  case 190: {
    using namespace hnco::representation;
    using Fn = Sudoku;
    using Conv = ScalarToDouble<double>;
    auto instance = new Fn();
    instance->load(_options.get_path());
    switch (_options.get_rep_categorical_representation()) {
    case 0: {
      using Rep = IntegerCategoricalRepresentation;
      auto reps = std::vector<Rep>(instance->get_num_variables(), Rep(9));
      return new MultivariateFunctionAdapter<Fn, Rep, Conv>(instance, reps);
    }
    case 1: {
      using Rep = LinearCategoricalRepresentation;
      auto reps = std::vector<Rep>(instance->get_num_variables(), Rep(9));
      return new MultivariateFunctionAdapter<Fn, Rep, Conv>(instance, reps);
    }
    default:
      throw std::runtime_error("CommandLineFunctionFactory::make: Unknown categorical representation type: " + std::to_string(_options.get_rep_categorical_representation()));
    }
  }

  case 200: {
    using namespace hnco::representation;
    auto instance = new Tsp();
    instance->load(_options.get_path());
    int num_elements = instance->get_num_elements();
    return new PermutationFunctionAdapter<Tsp>
      (instance,
       PermutationRepresentation(num_elements, _options.get_rep_num_additional_bits()));
  }

#ifdef ENABLE_PLUGIN
  case 1000:
    return new FunctionPlugin
      (_options.get_bv_size(),
       _options.get_path(),
       _options.get_fn_name());
#endif

#ifdef ENABLE_PYTHON
  case 1100:
    return new PythonFunction
      (_options.get_path(),
       _options.get_fn_name());
#endif

  default:
    throw std::runtime_error("CommandLineFunctionFactory::make: Unknown function type: " + std::to_string(_options.get_function()));
  }

}
