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

#include "config.h"

#include "hnco/functions/all.hh"

#include "function-factory.hh"
#include "make-multivariate-function-adapter.hh"

using namespace hnco::app;
using namespace hnco::function;
using namespace hnco::representation;
using namespace hnco;

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
    instance->generate(_options.get_bv_size(),
                       [w = 1] () mutable { double result = w; w *= 2; return result; });
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
    using Fn      = ParsedMultivariateFunction<FunctionParser>;
    using Rep     = DyadicFloatRepresentation<double>;
    using Conv    = ScalarToDouble<double>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return make_multivariate_function_adapter<HncoOptions, Adapter>(_options);
  }

  case 181: {
    using Fn      = ParsedMultivariateFunction<FunctionParser_li>;
    using Rep     = DyadicIntegerRepresentation<long>;
    using Conv    = ScalarToDouble<long>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return make_multivariate_function_adapter<HncoOptions, Adapter>(_options);
  }

  case 182: {
    using Fn      = ParsedMultivariateFunction<FunctionParser_cd>;
    using Rep     = ComplexRepresentation<DoubleRep>;
    using Conv    = ComplexToDouble<double>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return make_multivariate_function_adapter_complex<HncoOptions, Adapter>(_options);
  }

  case 183: {
    using Fn      = ParsedMultivariateFunction<FunctionParser>;
    using Rep     = DyadicIntegerRepresentation<int>;
    using Conv    = ScalarToDouble<double>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return make_multivariate_function_adapter<HncoOptions, Adapter>(_options);
  }

  case 184: {
    using Fn      = ParsedMultivariateFunction<FunctionParser>;
    using Conv    = ScalarToDouble<double>;
    using Adapter = MixedRepresentationMultivariateFunctionAdapter<Fn, rep_var_t, Conv>;
    return make_mixed_type_multivariate_function_adapter<HncoOptions, Adapter>(_options);
  }

  case 190: {
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
      throw std::runtime_error
        ("CommandLineFunctionFactory::make: Unknown categorical representation type: "
         + std::to_string(_options.get_rep_categorical_representation()));
    }
  }

  case 200: {
    auto instance = new Tsp();
    instance->load(_options.get_path());
    int num_elements = instance->get_num_elements();
    return new PermutationFunctionAdapter<Tsp>
      (instance,
       PermutationRepresentation(num_elements, _options.get_rep_num_additional_bits()));
  }

  case 210: {
    using Fn      = ParsedMultivariateFunction<FunctionParser>;
    using Rep     = DyadicFloatRepresentation<double>;
    using Conv    = EquationScalarToDouble<double>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return make_multivariate_function_adapter<HncoOptions, Adapter>(_options);
  }

  case 211: {
    using Fn      = ParsedMultivariateFunction<FunctionParser_li>;
    using Rep     = DyadicIntegerRepresentation<long>;
    using Conv    = EquationScalarToDouble<long>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return make_multivariate_function_adapter<HncoOptions, Adapter>(_options);
  }

  case 212: {
    using Fn      = ParsedMultivariateFunction<FunctionParser_cd>;
    using Rep     = ComplexRepresentation<DoubleRep>;
    using Conv    = EquationComplexToDouble<double>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return make_multivariate_function_adapter_complex<HncoOptions, Adapter>(_options);
  }

  case 213: {
    using Fn      = ParsedMultivariateFunction<FunctionParser>;
    using Rep     = DyadicIntegerRepresentation<int>;
    using Conv    = EquationScalarToDouble<double>;
    using Adapter = MultivariateFunctionAdapter<Fn, Rep, Conv>;
    return make_multivariate_function_adapter<HncoOptions, Adapter>(_options);
  }

  case 214: {
    using Fn      = ParsedMultivariateFunction<FunctionParser>;
    using Conv    = EquationScalarToDouble<double>;
    using Adapter = MixedRepresentationMultivariateFunctionAdapter<Fn, rep_var_t, Conv>;
    return make_mixed_type_multivariate_function_adapter<HncoOptions, Adapter>(_options);
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
    throw std::runtime_error
      ("CommandLineFunctionFactory::make: Unknown function type: "
       + std::to_string(_options.get_function()));
  }

}
