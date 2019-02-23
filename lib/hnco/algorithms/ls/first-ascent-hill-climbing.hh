/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_LS_FIRST_ASCENT_HILL_CLIMBING_H
#define HNCO_ALGORITHMS_LS_FIRST_ASCENT_HILL_CLIMBING_H

#include <functional>           // std::function

#include "hnco/algorithms/algorithm.hh"
#include "hnco/neighborhoods/neighborhood-iterator.hh"


namespace hnco {
namespace algorithm {


  /// First ascent hill climbing
  class FirstAscentHillClimbing:
    public IterativeAlgorithm {

  protected:

    /// Neighborhood
    neighborhood::NeighborhoodIterator *_neighborhood;

    /// Single iteration
    void iterate();

  public:

    /// Constructor
    FirstAscentHillClimbing(int n, neighborhood::NeighborhoodIterator *neighborhood):
      IterativeAlgorithm(n),
      _neighborhood(neighborhood) {}

    /// Random initialization
    void init();

    /// Explicit initialization
    void init(const bit_vector_t& x);

    /// Explicit initialization
    void init(const bit_vector_t& x, double value);

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
