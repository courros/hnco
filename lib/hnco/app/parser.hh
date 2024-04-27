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

#ifndef HNCO_APP_PARSER_H
#define HNCO_APP_PARSER_H

#include <cassert>

#include <unordered_map>
#include <type_traits>          // std::is_same_v
#include <variant>              // std::variant, std::get, std::holds_alternative

#include "hnco/representations/all.hh"

namespace hnco {
namespace app {

/**
 * Read file content
 */
std::string read_file_content(std::string path);

/**
 * Split string
 */
std::vector<std::string>
split_string(std::string str, std::string delimiter);

/// Int representation
using IntRep = representation::DyadicIntegerRepresentation<int>;
/// Long representation
using LongRep = representation::DyadicIntegerRepresentation<long>;
/// Double representation
using DoubleRep = representation::DyadicFloatRepresentation<double>;
/// Value set representation
using ValueSetRep = representation::ValueSetRepresentation<double>;

struct IntRepParams
{
  int lower_bound;
  int upper_bound;
  IntRepParams() = default;
  IntRepParams(int a, int b)
    : lower_bound(a)
    , upper_bound(b) {}
  IntRep to_rep() { return IntRep(lower_bound, upper_bound); }
};

struct LongRepParams
{
  long lower_bound;
  long upper_bound;
  LongRepParams() = default;
  LongRepParams(long a, long b)
    : lower_bound(a)
    , upper_bound(b) {}
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
  DoubleRepParams(double a, double b, double c)
    : lower_bound(a)
    , upper_bound(b)
    , precision(c)
    , mode(1) {}
  DoubleRepParams(double a, double b, int c)
    : lower_bound(a)
    , upper_bound(b)
    , size(c)
    , mode(2) {}
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

struct ValueSetRepParams
{
  std::vector<double> values;
  ValueSetRepParams() = default;
  ValueSetRepParams(std::vector<double> vs): values(vs) {}
  ValueSetRep to_rep() { return ValueSetRep(values); }
};

using param_var_t = std::variant<IntRepParams, LongRepParams, DoubleRepParams, ValueSetRepParams>;

using env_t = std::unordered_map<std::string, param_var_t>;

template<typename Options>
std::string get_expression(const Options& options)
{
  if (options.with_fp_expression())
    return options.get_fp_expression();
  else
    return read_file_content(options.get_path());
}

template<typename Options>
std::string get_representations(const Options& options)
{
  switch (options.get_fp_representations_source()) {
  case 0:
    if (options.with_fp_representations())
      return options.get_fp_representations();
    else
      return {};
  case 1:
    return read_file_content(options.get_fp_representations_path());
  default:
    throw std::runtime_error("get_representations: Unknown source: "
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
  double a = std::stod(parameters[0]);
  double b = std::stod(parameters[1]);
  if (parameters.size() == 2) {
    if (options.with_fp_default_double_precision())
      return DoubleRepParams(a, b, options.get_fp_default_double_precision());
    else if (options.with_fp_default_double_size())
      return DoubleRepParams(a, b, options.get_fp_default_double_size());
    else
      throw std::runtime_error("parse_double_rep: Missing precision or size parameter");
  }
  assert(parameters.size() == 3);
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

ValueSetRepParams
parse_value_set_rep(std::string expression);

/**
 * Parse a representation.
 * @param expression Expression to parse
 * @param options Options
 */
template<typename Options>
param_var_t
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
  else if (type == "set")
    return parse_value_set_rep(str2);
  else
    throw std::runtime_error("parse_representation: Unknown representation type");
}

/**
 * Parse representations.
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
 * - set(x1, x2, ..., xn) where all xi's are double and n is a non zero natural
 *
 * Example:
 *
 * "x: double(0, 1); y: double(0, 1, precision = 1e-3); z: double(0, 1, size = 8); u: int(-10, 10); v: long(-100, 100); w: set(1.1, 2.2, 3.3)"
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
    try {
      env[name] = parse_representation<Options>(declaration.substr(stop + delimiter.length()), options);
    }
    catch (std::runtime_error& e) {
      throw std::runtime_error(name + ": " + e.what());
    }
  }
  return env;
}

template<typename Options, typename Rep>
param_var_t
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

}
}

#endif
