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

#ifndef HNCO_ALGORITHMS_LS_FIRST_ASCENT_HILL_CLIMBING_H
#define HNCO_ALGORITHMS_LS_FIRST_ASCENT_HILL_CLIMBING_H

#include "hnco/neighborhoods/neighborhood-iterator.hh"

#include "local-search-algorithm.hh"


namespace hnco {
namespace algorithm {


/// First ascent hill climbing
class FirstAscentHillClimbing:
    public LocalSearchAlgorithm<neighborhood::NeighborhoodIterator> {

protected:

  /// Single iteration
  void iterate() override;

public:

  /// Constructor
  FirstAscentHillClimbing(int n, neighborhood::NeighborhoodIterator *neighborhood):
    LocalSearchAlgorithm<neighborhood::NeighborhoodIterator>(n, neighborhood) {}

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
