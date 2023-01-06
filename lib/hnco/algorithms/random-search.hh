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

#ifndef HNCO_ALGORITHMS_RANDOM_SEARCH_H
#define HNCO_ALGORITHMS_RANDOM_SEARCH_H

#include "hnco/algorithms/iterative-algorithm.hh"


namespace hnco {
namespace algorithm {

/// Random search
class RandomSearch:
    public IterativeAlgorithm {

protected:

  /// Candidate
  bit_vector_t _candidate;

  /** @name Loop
   */
  ///@{

  /// Initialize
  void init() override;

  /// Single iteration
  void iterate() override;

  ///@}

public:

  /// Constructor
  RandomSearch(int n):
    IterativeAlgorithm(n),
    _candidate(n) {}

};

} // end of namespace algorithm
} // end of namespace hnco


#endif
