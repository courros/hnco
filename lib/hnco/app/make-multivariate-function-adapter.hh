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

#ifndef HNCO_APP_MAKE_MULTIVARIATE_FUNCTION_ADAPTER_H
#define HNCO_APP_MAKE_MULTIVARIATE_FUNCTION_ADAPTER_H

#define _USE_MATH_DEFINES
#include <cmath>                // M_PI, M_E

#include "parser.hh"

namespace hnco {
namespace app {

template<typename Rep>
Rep
param_var_to_rep(param_var_t v);

template<>
inline
IntRep
param_var_to_rep(param_var_t v) { return std::get<IntRepParams>(v).to_rep(); }

template<>
inline
LongRep
param_var_to_rep(param_var_t v) { return std::get<LongRepParams>(v).to_rep(); }

template<>
inline
DoubleRep
param_var_to_rep(param_var_t v) { return std::get<DoubleRepParams>(v).to_rep(); }

template<>
inline
ValueSetRep
param_var_to_rep(param_var_t v) { return std::get<ValueSetRepParams>(v).to_rep(); }

/**
 * Make a multivariate function adapter
 */
template<typename Options, typename Adapter>
Adapter *
make_multivariate_function_adapter(const Options& options)
{
  using Fn  = typename Adapter::function_type;
  using Rep = typename Adapter::representation_type;

  auto instance = new Fn(get_expression<Options>(options));
  if constexpr(std::is_same<typename Fn::domain_type, double>::value) {
    instance->add_constant("pi", M_PI);
    instance->add_constant("e", M_E);
  }
  instance->parse();

  env_t env = parse_representations<Options>(get_representations<Options>(options), options);
  auto default_representation = get_default_representation<Options, Rep>(options);
  for (const auto& name : instance->get_variable_names()) {
    if (env.count(name) == 0) {
      env[name] = default_representation;
      std::cerr << "make_multivariate_function_adapter: Missing representation for " << name << ". Using default representation." << std::endl;
    } else if (env[name].index() != default_representation.index()) {
      env[name] = default_representation;
      std::cerr << "make_multivariate_function_adapter: Representation mismatch for " << name << ". Using default representation." << std::endl;
    }
  }

  std::vector<Rep> reps;
  for (const auto& name : instance->get_variable_names()) {
    assert(env.count(name) == 1);
    reps.push_back(param_var_to_rep<Rep>(env[name]));
  }

  return new Adapter(instance, reps);
}

/**
 * Make a multivariate function adapter over complex domain.
 */
template<typename Options, typename Adapter>
Adapter *
make_multivariate_function_adapter_complex(const Options& options)
{
  using Fn        = typename Adapter::function_type;
  using Rep       = typename Adapter::representation_type;
  using ScalarRep = typename Rep::scalar_rep;

  auto instance = new Fn(get_expression<Options>(options));
  instance->add_constant("pi", M_PI);
  instance->add_constant("e", M_E);
  instance->parse();

  env_t env = parse_representations<Options>(get_representations<Options>(options), options);
  auto default_representation = get_default_representation<Options, ScalarRep>(options);
  for (const auto& name : instance->get_variable_names()) {
    auto complete = [&](std::string str) -> void {
      if (env.count(str) == 0) {
        env[str] = default_representation;
        std::cerr << "make_multivariate_function_adapter: Missing representation for " << str << ". Using default representation." << std::endl;
      } else if (env[str].index() != default_representation.index()) {
        env[str] = default_representation;
        std::cerr << "make_multivariate_function_adapter: Representation mismatch for " << str << ". Using default representation." << std::endl;
      }
    };
    complete(name + "_re");
    complete(name + "_im");
  }

  std::vector<Rep> reps;
  for (const auto& name : instance->get_variable_names()) {
    assert(env.count(name + "_re") == 1);
    assert(env.count(name + "_im") == 1);
    reps.push_back(Rep(param_var_to_rep<ScalarRep>(env[name + "_re"]),
                       param_var_to_rep<ScalarRep>(env[name + "_im"])));
  }

  return new Adapter(instance, reps);
}

/// Representation variant
using rep_var_t = std::variant<IntRep, LongRep, DoubleRep, ValueSetRep>;

/**
 * Make a mixed-type multivariate function adapter
 */
template<typename Options, typename Adapter>
Adapter *
make_mixed_type_multivariate_function_adapter(const Options& options)
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
      std::cerr << "make_mixed_type_multivariate_function_adapter: Missing representation for " << name << ". Using default double representation." << std::endl;
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
