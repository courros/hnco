/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#ifndef HNCO_PERMUTATION_H
#define HNCO_PERMUTATION_H

#include <vector>
#include <algorithm>            // shuffle

#include "random.hh"


namespace hnco {

  /** @name Types and functions related to permutations
   */
  ///@{

  /// Permutation type
  typedef std::vector<std::size_t> permutation_t;

  /// Check that a vector represents a permutation
  bool perm_is_valid(const permutation_t& permutation);

  /// Sample a random permutation
  inline void perm_random(permutation_t& s)
  {
    for (std::size_t i = 0; i < s.size(); i++)
      s[i] = i;
    shuffle(s.begin(), s.end(), random::Random::engine);
  }

  ///@}

} // end of namespace hnco


#endif
