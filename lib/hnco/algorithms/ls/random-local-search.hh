/* Copyright (C) 2016, 2017 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_LS_LOCAL_SEARCH_H
#define HNCO_ALGORITHMS_LS_LOCAL_SEARCH_H

#include <assert.h>

#include <vector>

#include "hnco/algorithms/algorithm.hh"
#include "hnco/exception.hh"

#include "neighborhood.hh"


namespace hnco {
namespace algorithm {


  /// Abstract random local search
  class RandomLocalSearch:
    public IterativeAlgorithm {

  protected:

    /// Neighborhood
    neighborhood::Neighborhood *_neighborhood;

    /// Number of failure
    int _failures;

  public:

    /// Constructor
    RandomLocalSearch(int n, neighborhood::Neighborhood *neighborhood):
      IterativeAlgorithm(n),
      _neighborhood(neighborhood) {}

    /** Patience.

        Number of consecutive rejected moves before throwing a
        LocalMaximum exception

        If patience <= 0 then patience is considered infinite, meaning
        that the algorithm will never throw any LocalMaximum
        exception.
    */
    int _patience = 50;

    /// Random initialization
    void init();

    /// Explicit initialization
    void init(const bit_vector_t& x);

    /// Explicit initialization
    void init(const bit_vector_t& x, double value);

    /// Solution
    const point_value_t& get_solution();

  };

  /// Strict random local search
  class StrictRandomLocalSearch:
    public RandomLocalSearch {

    /** Single iteration

        \warning iterate does not update _solution; it only updates
        _maximum. The actual solution is embedded in the Neighborhood
        object and can be retrieved with get_solution.
    */
    void iterate();

  public:

    /// Constructor
    StrictRandomLocalSearch(int n,
                            neighborhood::Neighborhood *neighborhood):
      RandomLocalSearch(n, neighborhood) {}

  };

  /// Non strict random local search
  class NonStrictRandomLocalSearch:
    public RandomLocalSearch {

    /** Single iteration

        \warning iterate does not update _solution; it only updates
        _maximum. The actual solution is embedded in the Neighborhood
        object and can be retrieved with get_solution.
    */
    void iterate();

  public:

    /// Constructor
    NonStrictRandomLocalSearch(int n,
                               neighborhood::Neighborhood *neighborhood):
      RandomLocalSearch(n, neighborhood) {}

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
