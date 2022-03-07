/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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
#include <stdexcept>            // std::runtime_error

#include "config.h"

#include "hnco/functions/converter.hh"
#include "hnco/multiobjective/functions/all.hh"

#include "function-factory.hh"

using namespace hnco::multiobjective::app;
using namespace hnco::multiobjective::function;
using namespace hnco;


Function *
CommandLineFunctionFactory::make()
{
  if (_options.get_bv_size() <= 0)
    throw std::runtime_error("CommandLineFunctionFactory::make: bv_size must be positive");

  switch(_options.get_function()) {

  case 180: {
    using namespace hnco::representation;
    using Rep = DyadicFloatRepresentation<double>;
    using Fn = ParsedMultivariateFunction<FunctionParser>;
    using Conv = hnco::function::ScalarToDouble<double>;
    auto instance = new Fn(_options.get_fp_expression());
    if (_options.set_fp_num_bits()) {
      auto reps = std::vector<Rep>
        (instance->get_num_variables(),
         Rep(_options.get_fp_lower_bound(),
             _options.get_fp_upper_bound(),
             _options.get_fp_num_bits()));
      return new MultivariateFunctionAdapter<Fn, Rep, Conv>(instance, reps);
    } else {
      auto reps = std::vector<Rep>
        (instance->get_num_variables(),
         Rep(_options.get_fp_lower_bound(),
             _options.get_fp_upper_bound(),
             _options.get_fp_precision()));
      return new MultivariateFunctionAdapter<Fn, Rep, Conv>(instance, reps);
    }
  }

  case 181: {
    using namespace hnco::representation;
    using Rep = DyadicIntegerRepresentation<long>;
    using Fn = ParsedMultivariateFunction<FunctionParser_li>;
    using Conv = hnco::function::ScalarToDouble<long>;
    auto instance = new Fn(_options.get_fp_expression());
    auto reps = std::vector<Rep>
      (instance->get_num_variables(),
       Rep(_options.get_fp_lower_bound(),
           _options.get_fp_upper_bound()));
    return new MultivariateFunctionAdapter<Fn, Rep, Conv>(instance, reps);
  }

  case 182: {
    using namespace hnco::representation;
    using FloatRep = DyadicFloatRepresentation<double>;
    using Rep = DyadicComplexRepresentation<double>;
    using Fn = ParsedMultivariateFunction<FunctionParser_cd>;
    using Conv = hnco::function::ComplexToDouble<double>;
    auto instance = new Fn(_options.get_fp_expression());
    if (_options.set_fp_num_bits()) {
      FloatRep float_rep(_options.get_fp_lower_bound(),
                       _options.get_fp_upper_bound(),
                       _options.get_fp_num_bits());
      auto reps = std::vector<Rep>(instance->get_num_variables(),
                                   Rep(float_rep, float_rep));
      return new MultivariateFunctionAdapter<Fn, Rep, Conv>(instance, reps);
    } else {
      FloatRep float_rep(_options.get_fp_lower_bound(),
                       _options.get_fp_upper_bound(),
                       _options.get_fp_precision());
      auto reps = std::vector<Rep>(instance->get_num_variables(),
                                   Rep(float_rep, float_rep));
      return new MultivariateFunctionAdapter<Fn, Rep, Conv>(instance, reps);
    }
  }

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
