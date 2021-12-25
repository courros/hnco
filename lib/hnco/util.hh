/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#include <math.h>               // std::exp

#include <string>               // std::string
#include <sstream>              // std::ostringstream


namespace hnco {


/** @name Range checking
 */
///@{

/** Check whether an index is in a given range.

    \param i Index
    \param a Lower bound
    \param b Upper bound (excluded)

    \return true if i >= a and i < b
*/
inline bool is_in_range(int i, int a, int b) { return i >= a && i < b; }

/** Check whether an index is in a given range.

    The lower bound is implicit and is equal to 0.

    \param i Index
    \param n Upper bound (excluded)

    \return true if i >= 0 and i < n
*/
inline bool is_in_range(int i, int n) { return is_in_range(i, 0, n); }

///@}

/** @name Intervals
 */
///@{

/// Check whether a float value belongs to a given interval
inline bool is_in_interval(double x, double a, double b) { return x >= a && x <= b; }

/// Clip value between two bounds
template<typename T>
T clip_value(T x, T low, T high)
{
  assert(low < high);

  if (x > high)
    return high;
  else if (x < low)
    return low;
  else
    return x;
}

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
