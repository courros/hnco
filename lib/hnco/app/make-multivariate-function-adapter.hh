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

#include <iostream>
#include <optional>
#include <sstream>
#include <unordered_map>
#include <type_traits>          // std::is_same

#include "size-parser.hh"

namespace hnco {
namespace app {

/// Interval
template<typename T>
using Interval = std::pair<T, T>;

/// Insert an interval
template<typename T>
std::ostream& operator<<(std::ostream& stream, const Interval<T> interval)
{
  stream << "[" << interval.first << ", " << interval.second << "]";
  return stream;
}

/**
 * Parse an interval.
 *
 * Format: "[scalar, scalar]"
 *
 * Example : "[0, 1]"
 */
template<typename T>
std::optional<Interval<T>> parse_interval(std::string expression)
{
  if (expression.empty())
    return {};

  std::istringstream stream(expression);

  char c;
  stream >> c;
  if (!stream || c != '[') {
    std::cerr << "parse_interval: Expect [" << std::endl;
    return {};
  }

  Interval<T> interval;

  stream >> interval.first;
  if (!stream) {
    std::cerr << "parse_interval: Expect lower bound" << std::endl;
    return {};
  }

  stream >> c;
  if (!stream || c != ',') {
    std::cerr << "parse_interval: Expect ," << std::endl;
    return {};
  }

  stream >> interval.second;
  if (!stream) {
    std::cerr << "parse_interval: Expect upper bound" << std::endl;
    return {};
  }

  if (!(interval.first < interval.second)) {
    std::cerr << "parse_interval: Lower bound must be lower than upper bound" << std::endl;
    return {};
  }

  stream >> c;
  if (!stream || c != ']') {
    std::cerr << "parse_interval: Expect ]" << std::endl;
    return {};
  }

  return interval;
}

/**
 * Parse an interval declaration.
 *
 * Format: "name: [a, b]"
 *
 * Example : "x: [0, 1]"
 */
template<typename T>
std::optional<std::pair<std::string, Interval<T>>> parse_interval_declaration(std::string expression)
{
  const std::string delimiter = ":";

  if (expression.empty())
    return {};

  auto start = 0U;
  auto stop = expression.find(delimiter);

  if (stop == std::string::npos) {
    std::cerr << "parse_interval_declaration: Missing colon" << std::endl;
    return {};
  }

  auto before = expression.substr(start, stop);
  std::istringstream stream(before);
  std::string name;
  stream >> name;
  if (!stream) {
    std::cerr << "parse_interval_declaration: Expect variable name before colon" << std::endl;
    return {};
  }

  start = stop + delimiter.length();
  auto opt = parse_interval<T>(expression.substr(start));
  if (opt)
    return std::make_pair(name, opt.value());
  else {
    std::cerr << "parse_interval_declaration: Failed to parse interval for variable " << name << std::endl;
    return {};
  }

}

/**
 * Parse intervals
 *
 * Format: list of interval declarations separated by semicolon
 *
 * Example: "x: [0, 1]; y: [1, 2]"
 */
template<typename T>
std::unordered_map<std::string, Interval<T>> parse_intervals(std::string expression)
{
  const std::string delimiter = ";";

  std::unordered_map<std::string, Interval<T>> intervals;

  if (expression.empty())
    return intervals;

  auto start = 0U;
  auto stop = expression.find(delimiter);

  while (stop != std::string::npos) {
    auto opt = parse_interval_declaration<T>(expression.substr(start, stop - start));
    if (opt)
      intervals.insert(opt.value());
    start = stop + delimiter.length();
    stop = expression.find(delimiter, start);
  }

  auto opt = parse_interval_declaration<T>(expression.substr(start, stop - start));
  if (opt)
    intervals.insert(opt.value());

  return intervals;
}

/**
 * Parse a precision declaration.
 *
 * Format: "string: floating point number"
 *
 * Example : "x: 1e-3"
 */
template<typename T>
std::optional<std::pair<std::string, T>> parse_precision_declaration(std::string expression)
{
  const std::string delimiter = ":";

  if (expression.empty())
    return {};

  auto start = 0U;
  auto stop = expression.find(delimiter);

  if (stop == std::string::npos) {
    std::cerr << "parse_precision_declaration: Missing colon" << std::endl;
    return {};
  }

  std::string name;
  {
    auto before = expression.substr(start, stop);
    std::istringstream stream(before);
    stream >> name;
    if (!stream) {
      std::cerr << "parse_precision_declaration: Expect variable name before colon" << std::endl;
      return {};
    }
  }

  start = stop + delimiter.length();
  T precision;
  {
    auto after = expression.substr(start);
    std::istringstream stream(after);
    stream >> precision;
    if (!stream) {
      std::cerr << "parse_precision_declaration: "  << name << ": Expect precision after colon" << std::endl;
      return {};
    }
  }

  return std::make_pair(name, precision);

}

/**
 * Parse precisions
 *
 * Format: list of precision declarations separated by semicolon
 *
 * Example: "x: 1e-3; y: 0.01"
 */
template<typename T>
std::unordered_map<std::string, T> parse_precisions(std::string expression)
{
  const std::string delimiter = ";";

  std::unordered_map<std::string, T> precisions;

  if (expression.empty())
    return precisions;

  auto start = 0U;
  auto stop = expression.find(delimiter);

  while (stop != std::string::npos) {
    auto opt = parse_precision_declaration<T>(expression.substr(start, stop - start));
    if (opt)
      precisions.insert(opt.value());
    start = stop + delimiter.length();
    stop = expression.find(delimiter, start);
  }

  auto opt = parse_precision_declaration<T>(expression.substr(start, stop - start));
  if (opt)
    precisions.insert(opt.value());

  return precisions;
}

/**
 * Retrieve interval.
 */
template<typename T>
Interval<T>
retrieve_interval(std::string name, const std::unordered_map<std::string, Interval<T>>& intervals, Interval<T> default_interval)
{
  if (intervals.count(name)) {
    return intervals.at(name);
  } else {
    std::cerr
      << "Warning: retrieve_interval: No interval for " << name
      << " hence using default interval " << default_interval << std::endl;
    return default_interval;
  }
}

/**
 * Make a representation.
 */
template<typename Options, typename Rep>
auto
make_representation(std::string name,
                    Interval<typename Rep::domain_type> interval,
                    const std::unordered_map<std::string, typename Rep::domain_type>& precisions,
                    const std::unordered_map<std::string, int>& sizes,
                    const Options& options)
{
  if (precisions.count(name)) {
    std::cerr << name << ": Using given precision" << std::endl;
    return Rep(interval.first, interval.second, precisions.at(name));
  } else if (sizes.count(name)) {
    std::cerr << name << ": Using given size" << std::endl;
    return Rep(interval.first, interval.second, sizes.at(name));
  } else if (options.with_fp_default_precision()) {
    std::cerr << name << ": Using default precision" << std::endl;
    return Rep(interval.first, interval.second, options.get_fp_default_precision());
  } else {
    std::cerr << name << ": Using default size" << std::endl;
    return Rep(interval.first, interval.second, options.get_fp_default_size());
  }
}

/**
 * Make a multivariate function adapter over float domain.
 */
template<typename Options, typename Adapter>
Adapter *
make_multivariate_function_adapter_float(const Options& options)
{
  using Fn    = typename Adapter::function_type;
  using Rep   = typename Adapter::representation_type;
  using Float = typename Rep::domain_type;

  auto instance = new Fn(options.get_fp_expression());

  Interval<Float> default_interval;
  auto opt = parse_interval<Float>(options.get_fp_default_interval());
  if (opt)
    default_interval = opt.value();
  else
    throw std::runtime_error
      ("make_multivariate_function_adapter_float: Bad default interval: "
       + options.get_fp_default_interval());

  auto intervals  = parse_intervals<Float>(options.get_fp_intervals());
  auto precisions = parse_precisions<Float>(options.get_fp_precisions());
  auto sizes      = parse_sizes(options.get_fp_sizes());

  std::vector<Rep> reps;
  for (const auto& name : instance->get_variable_names()) {
    Interval<Float> interval = retrieve_interval<Float>(name, intervals, default_interval);
    Rep rep = make_representation<Options, Rep>(name, interval, precisions, sizes, options);
    reps.push_back(rep);
  }

  return new Adapter(instance, reps);
}

/**
 * Make a multivariate function adapter over integer domain.
 */
template<typename Options, typename Adapter>
Adapter *
make_multivariate_function_adapter_integer(const Options& options)
{
  using Fn        = typename Adapter::function_type;
  using Rep       = typename Adapter::representation_type;
  using Integer   = typename Rep::domain_type;
  using Precision = typename Rep::Precision;

  auto instance = new Fn(options.get_fp_expression());

  Interval<Integer> default_interval;
  auto opt = parse_interval<Integer>(options.get_fp_default_interval());
  if (opt)
    default_interval = opt.value();
  else
    throw std::runtime_error
      ("make_multivariate_function_adapter_integer: Bad default interval: "
       + options.get_fp_default_interval());

  auto intervals  = parse_intervals<Integer>(options.get_fp_intervals());
  auto precisions = parse_precisions<Integer>(options.get_fp_precisions());
  auto sizes      = parse_sizes(options.get_fp_sizes());

  std::vector<Rep> reps;
  for (const auto& name : instance->get_variable_names()) {
    Interval<Integer> interval = retrieve_interval<Integer>(name, intervals, default_interval);

    if (precisions.count(name)) {
      std::cerr << name << ": Using given precision" << std::endl;
      reps.push_back(Rep(interval.first, interval.second, Precision(precisions.at(name))));
    } else if (sizes.count(name)) {
      std::cerr << name << ": Using given size" << std::endl;
      reps.push_back(Rep(interval.first, interval.second, sizes.at(name)));
    } else if (options.with_fp_default_precision()) {
      std::cerr << name << ": Using default precision" << std::endl;
      reps.push_back(Rep(interval.first, interval.second, options.get_fp_default_precision()));
    } else {
      std::cerr << name << ": Using default size" << std::endl;
      reps.push_back(Rep(interval.first, interval.second, options.get_fp_default_size()));
    }

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
  using Fn       = typename Adapter::function_type;
  using Rep      = typename Adapter::representation_type;
  using FloatRep = typename Rep::float_representation_type;
  using Float    = typename FloatRep::domain_type;

  auto instance = new Fn(options.get_fp_expression());

  Interval<Float> default_interval;
  auto opt = parse_interval<Float>(options.get_fp_default_interval());
  if (opt)
    default_interval = opt.value();
  else
    throw std::runtime_error
      ("make_multivariate_function_adapter_complex: Bad default interval: "
       + options.get_fp_default_interval());

  auto intervals  = parse_intervals<Float>(options.get_fp_intervals());
  auto precisions = parse_precisions<Float>(options.get_fp_precisions());
  auto sizes      = parse_sizes(options.get_fp_sizes());

  std::vector<Rep> reps;
  for (const auto& name : instance->get_variable_names()) {
    std::string name_re = name + "_re";
    std::string name_im = name + "_im";
    Interval<Float> interval_re = retrieve_interval<Float>(name_re, intervals, default_interval);
    Interval<Float> interval_im = retrieve_interval<Float>(name_im, intervals, default_interval);
    FloatRep rep_re = make_representation<Options, FloatRep>(name_re, interval_re, precisions, sizes, options);
    FloatRep rep_im = make_representation<Options, FloatRep>(name_im, interval_im, precisions, sizes, options);
    reps.push_back(Rep(rep_re, rep_im));
  }

  return new Adapter(instance, reps);
}

}
}

#endif
