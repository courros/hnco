/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_HUMAN_H
#define HNCO_ALGORITHMS_HUMAN_H

#include "hnco/algorithms/iterative-algorithm.hh"


namespace hnco {
namespace algorithm {


/// %Human
class Human: public IterativeAlgorithm {

protected:

  /// Candidate
  bit_vector_t _candidate;

  /// Parse bit vector
  void parse_bit_vector();

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
  Human(int n)
    : IterativeAlgorithm(n)
    , _candidate(n)
  {}

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
