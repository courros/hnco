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

#ifndef HNCO_MULTIOBJECTIVE_FUNCTIONS_VALUE_H
#define HNCO_MULTIOBJECTIVE_FUNCTIONS_VALUE_H

#include <assert.h>

#include <iostream>
#include <vector>

#include "hnco/util.hh"         // hnco::have_same_size, hnco::join


namespace hnco {
namespace multiobjective {
namespace function {


/** Value type.

    A value type is the type of the output of a Function in the
    context of multiobjective optimization.
*/
using value_t = std::vector<double>;

/** Domination relation.

    \param a First value
    \param b Second value

    \return true if a dominates b with respect to minimization
*/
inline bool dominates(const value_t& a, const value_t& b) {
  assert(have_same_size(a, b));
  bool result = false;
  for (size_t i = 0; i < a.size(); i++) {
    double diff = a[i] - b[i];
    if (diff > 0)
      return false;
    else if (diff < 0)
      result = true;
  }
  return result;
}

/// Display a value
inline void value_display(const value_t& a, std::ostream& stream)
{
  stream << "(" << join(a.begin(), a.end(), ", ") << ")";
}


} // end of namespace function
} // end of namespace multiobjective
} // end of namespace hnco


#endif
