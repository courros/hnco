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

#ifndef HNCO_ALGORITHMS_LS_RANDOM_LOCAL_SEARCH_H
#define HNCO_ALGORITHMS_LS_RANDOM_LOCAL_SEARCH_H

#include <assert.h>

#include <functional>           // std::function

#include "hnco/algorithms/algorithm.hh"
#include "hnco/exception.hh"

#include "neighborhood.hh"


namespace hnco {
namespace algorithm {


  /// Random local search
  class RandomLocalSearch:
    public IterativeAlgorithm {

  protected:

    /// Neighborhood
    neighborhood::Neighborhood *_neighborhood;

    /// Number of failure
    int _failures;

    /// Single iteration
    void iterate();

    /// Single iteration with full evaluation
    void iterate_full();

    /// Single iteration with incremental evaluation
    void iterate_incremental();

  public:

    /// Constructor
    RandomLocalSearch(int n, neighborhood::Neighborhood *neighborhood):
      IterativeAlgorithm(n),
      _neighborhood(neighborhood) {}

    /// Random initialization
    void init();

    /// Explicit initialization
    void init(const bit_vector_t& x);

    /// Explicit initialization
    void init(const bit_vector_t& x, double value);

    /// Solution
    const point_value_t& get_solution();

    /** @name Parameters
     */
    ///@{

    /// Binary operator for comparing evaluations
    std::function<bool(double, double)> _compare = std::greater_equal<double>();

    /// Incremental evaluation
    bool _incremental_evaluation = false;

    /** Patience.

        Number of consecutive rejected moves before throwing a
        LocalMaximum exception

        If patience <= 0 then patience is considered infinite, meaning
        that the algorithm will never throw any LocalMaximum
        exception.
    */
    int _patience = 50;

    ///@}

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
