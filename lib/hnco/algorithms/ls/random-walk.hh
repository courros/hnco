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

#ifndef HNCO_ALGORITHMS_LS_RANDOM_WALK_H
#define HNCO_ALGORITHMS_LS_RANDOM_WALK_H

#include "hnco/algorithms/algorithm.hh"
#include "hnco/exception.hh"

#include "hnco/neighborhoods/neighborhood.hh"


namespace hnco {
namespace algorithm {


  /** Random walk.

      The algorithm simply performs a random walk on the graph
      implicitly given by the neighborhood. At each iteration, the
      chosen neighbor does not depend on its evaluation. However
      optimization takes place as in random search, that is the best
      visited bit vector is remembered.
  */
  class RandomWalk:
    public IterativeAlgorithm {

  protected:

    /// Neighborhood
    neighborhood::Neighborhood *_neighborhood;

    /// Value of the last visited bit vector
    double _value;

    /** @name Parameters
     */
    ///@{

    /// Incremental evaluation
    bool _incremental_evaluation = false;

    ///@}

    /// Single iteration
    void iterate();

    /// Single iteration with full evaluation
    void iterate_full();

    /// Single iteration with incremental evaluation
    void iterate_incremental();

  public:

    /// Constructor
    RandomWalk(int n, neighborhood::Neighborhood *neighborhood):
      IterativeAlgorithm(n),
      _neighborhood(neighborhood) {}

    /// Random initialization
    void init();

    /// Explicit initialization
    void init(const bit_vector_t& x);

    /// Explicit initialization
    void init(const bit_vector_t& x, double value);

    /// Log
    void log();

    /** @name Setters
     */
    ///@{

    /// Set incremental evaluation
    void set_incremental_evaluation(bool x) { _incremental_evaluation = x; }

    /// Set log
    void set_log_value() { _something_to_log = true; }

    ///@}

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
