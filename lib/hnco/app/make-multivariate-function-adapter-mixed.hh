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

#include "parser.hh"

namespace hnco {
namespace app {

using rep_var_t = std::variant<IntRep, LongRep, DoubleRep, ValueSetRep>;

/**
 * Make a mixed-integer multivariate function adapter
 */
template<typename Options, typename Adapter>
Adapter *
make_multivariate_function_adapter_mixed(const Options& options)
{
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

  std::vector<rep_var_t> rep_variants;
  for (const auto& name : instance->get_variable_names()) {
    assert(env.count(name) == 1);
    rep_variants.push_back(std::visit([](auto&& arg) { return rep_var_t(arg.to_rep()); }, env[name]));
  }

  return new Adapter(instance, rep_variants);
}

}
}

#endif
