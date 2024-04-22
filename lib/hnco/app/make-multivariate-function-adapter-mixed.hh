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

#ifndef HNCO_APP_MAKE_MULTIVARIATE_FUNCTION_ADAPTER_MIXED_H
#define HNCO_APP_MAKE_MULTIVARIATE_FUNCTION_ADAPTER_MIXED_H

#define _USE_MATH_DEFINES
#include <cmath>                // M_PI, M_E

#include "hnco/representations/all.hh"

#include "parser.hh"

namespace hnco {
namespace app {

/**
 * Make a mixed-integer multivariate function adapter
 */
template<typename Options, typename Adapter>
Adapter *
make_multivariate_function_adapter_mixed(const Options& options)
{
  static_assert(std::is_same_v<typename Adapter::int_rep_type::domain_type, long>);
  static_assert(std::is_same_v<typename Adapter::float_rep_type::domain_type, double>);

  using Fn = typename Adapter::function_type;
  auto instance = new Fn(get_expression<Options>(options));
  instance->add_constant("pi", M_PI);
  instance->add_constant("e", M_E);
  instance->parse();

  env_t env = parse_representations<Options>(get_representations<Options>(options), options);
  auto default_long_rep = get_default_representation<Options, LongRep>(options);
  auto default_double_rep = get_default_representation<Options, DoubleRep>(options);
  for (const auto& name : instance->get_variable_names()) {
    if (env.count(name) == 0) {
      env[name] = default_double_rep;
      std::cerr << "make_multivariate_function_adapter_mixed: Missing representation for " << name << ". Using default double representation." << std::endl;
    } else {
      auto v = env[name];
      if (std::holds_alternative<IntRepParams>(v)) {
        env[name] = default_double_rep;
        std::cerr << "make_multivariate_function_adapter: Invalid representation for " << name << ". Using default double representation." << std::endl;
      } else {
        assert(std::holds_alternative<LongRepParams>(v) ||
               std::holds_alternative<DoubleRepParams>(v) ||
               std::holds_alternative<ValueSetRepParams>(v));
      }
    }
  }

  std::vector<LongRep> long_reps;
  std::vector<DoubleRep> double_reps;
  std::vector<ValueSetRep> value_set_reps;
  std::vector<std::pair<bool, int>> lut(instance->get_num_variables());
  int long_index = 0;
  int double_index = 0;
  int value_set_index = 0;
  int var_index = 0;
  for (const auto& name : instance->get_variable_names()) {
    assert(env.count(name) == 1);
    auto v = env[name];
    if (std::holds_alternative<LongRepParams>(v)) {
      long_reps.push_back(variant_to_rep<LongRep>(v));
      lut[var_index++] = std::make_pair<bool, int>(true, long_index++);
    } else if (std::holds_alternative<DoubleRepParams>(v)) {
      double_reps.push_back(variant_to_rep<DoubleRep>(v));
      lut[var_index++] = std::make_pair<bool, int>(false, double_index++);
    } else {
      assert(std::holds_alternative<ValueSetRepParams>(v));
      value_set_reps.push_back(variant_to_rep<ValueSetRep>(v));
      lut[var_index++] = std::make_pair<bool, int>(false, value_set_index++);
    }
  }
  assert(var_index == instance->get_num_variables());
  assert(var_index == long_index + double_index + value_set_index);

  return new Adapter(instance, long_reps, double_reps, lut);
}

}
}

#endif
