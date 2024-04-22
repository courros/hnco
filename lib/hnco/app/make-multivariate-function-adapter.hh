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
    reps.push_back(variant_to_rep<Rep>(env[name]));
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
    reps.push_back(Rep(variant_to_rep<ScalarRep>(env[name + "_re"]),
                       variant_to_rep<ScalarRep>(env[name + "_im"])));
  }

  return new Adapter(instance, reps);
}

}
}

#endif
