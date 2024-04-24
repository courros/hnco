/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#include "config.h"

#include "hnco/functions/converter.hh"
#include "hnco/multiobjective/functions/all.hh"
#include "hnco/app/make-multivariate-function-adapter.hh"

#include "function-factory.hh"

using namespace hnco::multiobjective::app;
using namespace hnco::multiobjective::function;
using namespace hnco::representation;
using namespace hnco;


Function *
CommandLineFunctionFactory::make()
{
  if (_options.get_bv_size() <= 0)
    throw std::runtime_error("CommandLineFunctionFactory::make: bv_size must be positive");

  switch(_options.get_function()) {

  case 180: {
    using Fn      = ParsedMultivariateFunction<FunctionParser>;
    using Rep     = DyadicFloatRepresentation<double>;
    using Conv    = hnco::function::ScalarToDouble<double>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return hnco::app::make_multivariate_function_adapter<HncoOptions, Adapter>(_options);
  }

  case 181: {
    using Fn      = ParsedMultivariateFunction<FunctionParser_li>;
    using Rep     = DyadicIntegerRepresentation<long>;
    using Conv    = hnco::function::ScalarToDouble<long>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return hnco::app::make_multivariate_function_adapter<HncoOptions, Adapter>(_options);
  }

  case 182: {
    using Fn      = ParsedMultivariateFunction<FunctionParser_cd>;
    using Rep     = ComplexRepresentation<hnco::app::DoubleRep>;
    using Conv    = hnco::function::ComplexToDouble<double>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return hnco::app::make_multivariate_function_adapter_complex<HncoOptions, Adapter>(_options);
  }

  case 183: {
    using Fn      = ParsedMultivariateFunction<FunctionParser>;
    using Rep     = DyadicIntegerRepresentation<int>;
    using Conv    = hnco::function::ScalarToDouble<double>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return hnco::app::make_multivariate_function_adapter<HncoOptions, Adapter>(_options);
  }

  case 184: {
    using Fn = ParsedMultivariateFunction<FunctionParser>;
    using Adapter = MixedTypeMultivariateFunctionAdapter<Fn, hnco::app::rep_var_t>;
    return hnco::app::make_mixed_type_multivariate_function_adapter<HncoOptions, Adapter>(_options);
  }

#ifdef ENABLE_PYTHON
  case 1100:
    return new PythonFunction
      (_options.get_path(),
       _options.get_fn_name());
#endif

  default:
    throw std::runtime_error("CommandLineFunctionFactory::make: Unknown function type: "
                             + std::to_string(_options.get_function()));
  }

}
