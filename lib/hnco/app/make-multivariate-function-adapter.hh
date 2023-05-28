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

namespace hnco {
namespace app {

template<typename T>
using Interval = std::pair<T, T>;

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Interval<T> interval)
{
  stream << "[" << interval.first << ", " << interval.second << "]";
  return stream;
}

template<typename T>
std::optional<Interval<T>> parse_interval(std::istringstream &stream)
{
  char c;
  stream >> c;
  if (stream.fail() || c != '[') {
    std::cerr << "parse_interval: Expected [" << std::endl;
    return {};
  }

  Interval<T> interval;

  stream >> interval.first;
  if (stream.fail()) {
    std::cerr << "parse_interval: Expected lower bound" << std::endl;
    return {};
  }

  stream >> c;
  if (stream.fail() || c != ',') {
    std::cerr << "parse_interval: Expected ," << std::endl;
    return {};
  }

  stream >> interval.second;
  if (stream.fail()) {
    std::cerr << "parse_interval: Expected upper bound" << std::endl;
    return {};
  }

  if (!(interval.first < interval.second)) {
    std::cerr << "parse_interval: Lower bound must be lower than upper bound" << std::endl;
    return {};
  }

  stream >> c;
  if (stream.fail() || c != ']') {
    std::cerr << "parse_interval: Expected ]" << std::endl;
    return {};
  }

  return interval;
}

template<typename T>
std::unordered_map<std::string, Interval<T>> parse_intervals(std::string str)
{
  std::unordered_map<std::string, Interval<T>> intervals;
  std::istringstream stream(str);

  while (stream.peek() != EOF) {
    std::string name;
    stream >> name;
    if (stream.fail()) {
      std::cerr << "parse_intervals: Expected variable name" << std::endl;
      break;
    }
    std::string keyword;
    stream >> keyword;
    if (stream.fail() || keyword != "in") {
      std::cerr << "parse_intervals: Expected keyword in followed by space after " << name << std::endl;
      break;
    }
    auto opt = parse_interval<T>(stream);
    if (opt)
      intervals[name] = opt.value();
    else
      std::cerr << "parse_intervals: Failed to parse interval for variable " << name << std::endl;
  };

  return intervals;
}

template<typename Options, typename Adapter>
Adapter *
make_multivariate_function_adapter_float(const Options& options)
{
  using Fn = typename Adapter::function_type;
  using Rep = typename Adapter::representation_type;
  using Float = typename Rep::domain_type;

  auto instance = new Fn(options.get_fp_expression());

  Interval<Float> default_interval;
  std::istringstream stream(options.get_fp_default_interval());
  auto opt = parse_interval<Float>(stream);
  if (opt)
    default_interval = opt.value();
  else
    throw std::runtime_error
      ("make_multivariate_function_adapter_float: Bad default interval: "
       + options.get_fp_default_interval());

  auto intervals = parse_intervals<Float>(options.get_fp_intervals());

  std::vector<Rep> reps;
  for (const auto& name : instance->get_variable_names()) {
    Interval<Float> interval;
    if (intervals.count(name)) {
      interval = intervals[name];
    } else {
      interval = default_interval;
      std::cerr
        << "Warning: make_multivariate_function_adapter_float: No interval for " << name
        << " hence using default interval " << options.get_fp_default_interval() << std::endl;
    }
    if (options.with_fp_precision())
      reps.push_back(Rep(interval.first,
                         interval.second,
                         options.get_fp_precision()));
    else
      reps.push_back(Rep(interval.first,
                         interval.second,
                         options.get_fp_num_bits()));
  }

  return new Adapter(instance, reps);
}

template<typename Options, typename Adapter>
Adapter *
make_multivariate_function_adapter_integer(const Options& options)
{
  using Fn = typename Adapter::function_type;
  using Rep = typename Adapter::representation_type;
  using Integer = typename Rep::domain_type;

  auto instance = new Fn(options.get_fp_expression());

  Interval<Integer> default_interval;
  std::istringstream stream(options.get_fp_default_interval());
  auto opt = parse_interval<Integer>(stream);
  if (opt)
    default_interval = opt.value();
  else
    throw std::runtime_error
      ("make_multivariate_function_adapter_integer: Bad default interval: "
       + options.get_fp_default_interval());

  auto intervals = parse_intervals<Integer>(options.get_fp_intervals());

  std::vector<Rep> reps;
  for (const auto& name : instance->get_variable_names()) {
    Interval<Integer> interval;
    if (intervals.count(name)) {
      interval = intervals[name];
    } else {
      interval = default_interval;
      std::cerr
        << "Warning: make_multivariate_function_adapter_integer: No interval for " << name
        << " hence using default interval " << options.get_fp_default_interval() << std::endl;
    }
    reps.push_back(Rep(interval.first, interval.second));
  }

  return new Adapter(instance, reps);
}

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
  std::istringstream stream(options.get_fp_default_interval());
  auto opt = parse_interval<Float>(stream);
  if (opt)
    default_interval = opt.value();
  else
    throw std::runtime_error
      ("make_multivariate_function_adapter_complex: Bad default interval: "
       + options.get_fp_default_interval());

  auto intervals = parse_intervals<Float>(options.get_fp_intervals());

  std::vector<Rep> reps;
  for (const auto& name : instance->get_variable_names()) {
    Interval<Float> interval_re, interval_im;
    std::string name_re = name + "_re";
    std::string name_im = name + "_im";
    if (intervals.count(name_re)) {
      interval_re = intervals[name_re];
    } else {
      interval_re = default_interval;
      std::cerr
        << "Warning: make_multivariate_function_adapter_complex: No interval for " << name_re
        << " hence using default interval " << options.get_fp_default_interval() << std::endl;
    }
    if (intervals.count(name_im)) {
      interval_im = intervals[name_im];
    } else {
      interval_im = default_interval;
      std::cerr
        << "Warning: make_multivariate_function_adapter_complex: No interval for " << name_im
        << " hence using default interval " << options.get_fp_default_interval() << std::endl;
    }
    if (options.with_fp_precision()) {
      FloatRep rep_re(interval_re.first,
                      interval_re.second,
                      options.get_fp_precision());
      FloatRep rep_im(interval_im.first,
                      interval_im.second,
                      options.get_fp_precision());
      reps.push_back(Rep(rep_re, rep_im));
    } else {
      FloatRep rep_re(interval_re.first,
                      interval_re.second,
                      options.get_fp_num_bits());
      FloatRep rep_im(interval_im.first,
                      interval_im.second,
                      options.get_fp_num_bits());
      reps.push_back(Rep(rep_re, rep_im));
    }

  }

  return new Adapter(instance, reps);
}

}
}

#endif