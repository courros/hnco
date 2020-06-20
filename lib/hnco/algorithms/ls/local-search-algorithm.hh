/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_LS_LOCAL_SEARCH_ALGORITHM_H
#define HNCO_ALGORITHMS_LS_LOCAL_SEARCH_ALGORITHM_H

#include "hnco/algorithms/iterative-algorithm.hh"
#include "hnco/neighborhoods/neighborhood.hh"


namespace hnco {
namespace algorithm {


/// Local search algorithm
class LocalSearchAlgorithm: public IterativeAlgorithm {

protected:

  /// Starting point
  bit_vector_t _starting_point;

  /// Neighborhood
  neighborhood::Neighborhood *_neighborhood;

  /** @name Parameters
   */
  ///@{

  /// Random initialization
  bool _random_initialization = true;

  ///@}

  /** @name Loop
   */
  ///@{

  /// Initialize
  void init() override;

  ///@}

public:

  /// Constructor
  LocalSearchAlgorithm(int n, neighborhood::Neighborhood *neighborhood):
    IterativeAlgorithm(n),
    _starting_point(n, 0),
    _neighborhood(neighborhood) {}

  /** @name Setters
   */
  ///@{

  /// Set random initialization
  void set_random_initialization(bool random_initialization) { _random_initialization = random_initialization; }

  /** Set the starting point.

      \warning Evaluates the function once.
  */
  void set_starting_point(const bit_vector_t& x) { _starting_point = x; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
