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

#ifndef HNCO_UTIL_H
#define HNCO_UTIL_H

#include <cassert>
#include <cmath>               // std::exp
#include <sstream>              // std::ostringstream
#include <string>               // std::string

namespace hnco {

/**
 * Ensure that a condition is satisfied or throw a runtime exception.
 * @param b Boolean
 * @param message Message to display if the boolean is false
 */
inline void ensure(bool b, const std::string message)
{
  if (!b)
    throw std::runtime_error(message);
}

/**
 * Fail with message and code
 * @param message Message
 * @param code Code
 */
[[noreturn]]
inline void fail_with(const std::string message, int code)
{
  throw std::runtime_error(message + std::to_string(code));
}

/**
 * @name Range checking
 */
///@{

/**
 * Check whether an index is in a given range.
 * @param i Index
 * @param a Lower bound
 * @param b Upper bound (excluded)
 * @return true if i >= a and i < b
 */
inline bool is_in_range(int i, int a, int b) { return i >= a && i < b; }

/**
 * Check whether an index is in a given range.
 * The lower bound is implicit and is equal to 0.
 * @param i Index
 * @param n Upper bound (excluded)
 * @return true if i >= 0 and i < n
 */
inline bool is_in_range(int i, int n) { return is_in_range(i, 0, n); }

///@}

/**
 * @name Intervals
 */
///@{

/// Check whether a double value belongs to a given interval
inline bool is_in_interval(double x, double a, double b) { return x >= a && x <= b; }

///@}

/// Check whether two containers have the same size
template<class A, class B>
bool have_same_size(const A& a, const B& b) { return a.size() == b.size(); }

/// Generic square function
template<class T>
T square(T x) { return x * x; }

/// Logistic function (sigmoid)
inline double logistic(double x)
{
  if (x > 0)
    return 1 / (1 + std::exp(-x));
  else {
    double tmp = std::exp(x);
    return tmp / (1 + tmp);
  }
}

/// Convert to string and join elements of a container (from SO)
template <typename Iter>
std::string join(Iter begin, Iter end, std::string const& separator)
{
  std::ostringstream result;
  if (begin != end)
    result << *begin++;
  while (begin != end)
    result << separator << *begin++;
  return result.str();
}

} // end of namespace hnco

#endif
