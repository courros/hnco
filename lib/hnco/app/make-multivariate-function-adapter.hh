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

#ifndef HNCO_APP_MAKE_MULTIVARIATE_FUNCTION_ADAPTER_H
#define HNCO_APP_MAKE_MULTIVARIATE_FUNCTION_ADAPTER_H

#include <assert.h>

#include <unordered_map>
#include <type_traits>          // std::is_same_v
#include <variant>              // std::variant, std::get

#include "hnco/representations/all.hh"

#define _USE_MATH_DEFINES
#include <cmath>                // M_PI, M_E

namespace hnco {
namespace app {

/**
 * Read file content
 */
std::string read_file_content(std::string path);

/**
 * Split string.
 */
std::vector<std::string>
split_string(std::string str, std::string delimiter);

/// Int representation
using IntRep    = representation::DyadicIntegerRepresentation<int>;

/// Long representation
using LongRep   = representation::DyadicIntegerRepresentation<long>;

/// Double representation
using DoubleRep = representation::DyadicFloatRepresentation<double>;

struct IntRepParams
{
  int lower_bound;
  int upper_bound;
  IntRepParams() = default;
  IntRepParams(int a, int b):
    lower_bound(a),
    upper_bound(b) {}
  IntRep to_rep() { return IntRep(lower_bound, upper_bound); }
};

struct LongRepParams
{
  long lower_bound;
  long upper_bound;
  LongRepParams() = default;
  LongRepParams(long a, long b):
    lower_bound(a),
    upper_bound(b) {}
  LongRep to_rep() { return LongRep(lower_bound, upper_bound); }
};

struct DoubleRepParams
{
  double lower_bound;
  double upper_bound;
  double precision;
  int size;
  int mode = 0;
  DoubleRepParams() = default;
  DoubleRepParams(double a, double b, double c):
    lower_bound(a),
    upper_bound(b),
    precision(c),
    mode(1) {}
  DoubleRepParams(double a, double b, int c):
    lower_bound(a),
    upper_bound(b),
    size(c),
    mode(2) {}
  DoubleRep to_rep() {
    switch (mode) {
    case 1:
      return DoubleRep(lower_bound, upper_bound, precision);
    case 2:
      return DoubleRep(lower_bound, upper_bound, size);
    default:
      throw std::runtime_error("DoubleRepParams::to_rep: Unknown mode");
    }
  }
};

using variant_t = std::variant<IntRepParams, LongRepParams, DoubleRepParams>;
using env_t = std::unordered_map<std::string, variant_t>;

template<typename Options>
std::string get_expression(const Options& options)
{
  switch (options.get_fp_expression_source()) {
  case 0:
    return options.get_fp_expression();
  case 1:
    return read_file_content(options.get_path());
  default:
    throw std::runtime_error("get_expression: Unknown source: "
                             + std::to_string(options.get_fp_expression_source()));
  }
}

template<typename Options>
std::string get_representations(const Options& options)
{
  switch (options.get_fp_representations_source()) {
  case 0:
    return options.get_fp_representations_source();
  case 1:
    return read_file_content(options.get_fp_representations_path());
  default:
    throw std::runtime_error("get_expression: Unknown source: "
                             + std::to_string(options.get_fp_representations_source()));
  }
}

IntRepParams
parse_int_rep(std::string expression);

LongRepParams
parse_long_rep(std::string expression);

template<typename Options>
DoubleRepParams
parse_double_rep(std::string expression, const Options& options)
{
  auto parameters = split_string(expression, ",");
  if (parameters.size() < 2)
    throw std::runtime_error("parse_double_rep: Not enough parameters");
  if (parameters.size() > 3)
    throw std::runtime_error("parse_double_rep: Too many parameters");
  if (parameters.size() == 2) {
    double a = std::stod(parameters[0]);
    double b = std::stod(parameters[1]);
    if (options.with_fp_default_double_precision())
      return DoubleRepParams(a, b, options.get_fp_default_double_precision());
    else if (options.with_fp_default_double_size())
      return DoubleRepParams(a, b, options.get_fp_default_double_size());
    else
      throw std::runtime_error("parse_double_rep: Missing precision or size parameter");
  }
  assert(parameters.size() == 3);
  double a = std::stod(parameters[0]);
  double b = std::stod(parameters[1]);
  auto terms = split_string(parameters[2], "=");
  if (terms.size() != 2)
    throw std::runtime_error("parse_double_rep: Invalid key value parameter");
  std::istringstream stream(terms[0]);
  std::string key;
  stream >> key;
  if (!stream)
    throw std::runtime_error("parse_double_rep: Missing or invalid key");
  if (key == "precision")
    return DoubleRepParams(a, b, std::stod(terms[1]));
  else if (key == "size")
    return DoubleRepParams(a, b, std::stoi(terms[1]));
  else
    throw std::runtime_error("parse_double_rep: Unknown key");
}

template<typename Options>
variant_t
parse_representation(std::string expression, const Options& options)
{
  if (expression.empty())
    throw std::runtime_error("parse_representation: Empty expression");

  auto openp = expression.find("(");
  if (openp == std::string::npos)
    throw std::runtime_error("parse_representation: Missing opening parenthesis");

  auto closep = expression.find(")", openp);
  if (closep == std::string::npos)
    throw std::runtime_error("parse_representation: Missing closing parenthesis");

  std::string str1 = expression.substr(0, openp);
  std::istringstream stream(str1);
  std::string type;
  stream >> type;
  if (!stream)
    throw std::runtime_error("parse_representation: Missing or invalid representation type");

  std::string str2 = expression.substr(openp + 1, closep - openp - 1);
  if (type == "int")
    return parse_int_rep(str2);
  else if (type == "long")
    return parse_long_rep(str2);
  else if (type == "double")
    return parse_double_rep<Options>(str2, options);
  else
    throw std::runtime_error("parse_representation: Unknown representation type");
}

/**
 * Parse representations
 *
 * @param expression Expression to parse
 * @param options Options
 *
 * Syntax:
 *
 * representations = declaration [; declaration]*
 *
 * declaration = name : representation
 *
 * representation =
 * - int(a, b) where a, b are int
 * - long(a, b) where a, b are long
 * - double(a, b, precision = e) where a, b, e are double
 * - double(a, b, size = n) where a, b are double, and n is int
 *
 * Example:
 *
 * "x: double(0, 1); y: double(0, 1, precision = 1e-3); z: double(0, 1, size = 8); u: int(-100, 100); v: long(1, 10000)"
 */
template<typename Options>
env_t
parse_representations(std::string expression, const Options& options)
{
  env_t env;
  auto declarations = split_string(expression, ";");
  for (auto declaration : declarations) {
    if (declaration.empty())
      continue;
    const std::string delimiter = ":";
    auto stop = declaration.find(delimiter);
    if (stop == std::string::npos)
      throw std::runtime_error("parse_representations: Missing colon");
    auto before = declaration.substr(0, stop);
    std::istringstream stream(before);
    std::string name;
    stream >> name;
    if (!stream)
      throw std::runtime_error("parse_representations: Missing variable name before colon");
    env[name] = parse_representation<Options>(declaration.substr(stop + delimiter.length()), options);
  }
  return env;
}

template<typename Options, typename Rep>
variant_t
get_default_representation(const Options& options)
{
  using T = typename Rep::domain_type;

  if constexpr (std::is_same_v<T, int>)
    return parse_representation<Options>(options.get_fp_default_int_rep(), options);
  else if constexpr (std::is_same_v<T, long>)
    return parse_representation<Options>(options.get_fp_default_long_rep(), options);
  else if constexpr  (std::is_same_v<T, double>)
    return parse_representation<Options>(options.get_fp_default_double_rep(), options);
  else
    throw std::runtime_error("get_default_representation: Unknown type");
}

template<typename Rep>
Rep
get_representation(variant_t v);

template<>
inline
IntRep
get_representation(variant_t v)
{
  return std::get<IntRepParams>(v).to_rep();
}

template<>
inline
LongRep
get_representation(variant_t v)
{
  return std::get<LongRepParams>(v).to_rep();
}

template<>
inline
DoubleRep
get_representation(variant_t v)
{
  return std::get<DoubleRepParams>(v).to_rep();
}

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

  env_t env;
  if (options.with_fp_representations())
    env = parse_representations<Options>(options.get_fp_representations(), options);
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
    reps.push_back(get_representation<Rep>(env[name]));
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

  env_t env;
  if (options.with_fp_representations())
    env = parse_representations<Options>(options.get_fp_representations(), options);
  auto default_representation = get_default_representation<Options, ScalarRep>(options);
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
    reps.push_back(get_representation<ScalarRep>(env[name + "_re"]));
    reps.push_back(get_representation<ScalarRep>(env[name + "_im"]));
  }

  return new Adapter(instance, reps);
}

}
}

#endif
