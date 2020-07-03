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

#ifndef HNCO_ALGORITHMS_LS_RANDOM_LOCAL_SEARCH_H
#define HNCO_ALGORITHMS_LS_RANDOM_LOCAL_SEARCH_H

#include <functional>           // std::function

#include "hnco/neighborhoods/neighborhood.hh"

#include "local-search-algorithm.hh"


namespace hnco {
namespace algorithm {


/// Random local search
class RandomLocalSearch:
    public LocalSearchAlgorithm<neighborhood::Neighborhood> {

protected:

  /// Number of failure
  int _num_failures;

  /** @name Parameters
   */
  ///@{

  /// Binary operator for comparing evaluations
  std::function<bool(double, double)> _compare = std::greater_equal<double>();

  /// Patience
  int _patience = 50;

  /// Incremental evaluation
  bool _incremental_evaluation = false;

  ///@}

  /** @name Loop
   */
  ///@{

  /// Initialize
  void init() override;

  /// Single iteration
  void iterate() override;

  ///@}

  /// Single iteration with full evaluation
  void iterate_full();

  /// Single iteration with incremental evaluation
  void iterate_incremental();

public:

  /// Constructor
  RandomLocalSearch(int n, neighborhood::Neighborhood *neighborhood):
    LocalSearchAlgorithm<neighborhood::Neighborhood>(n, neighborhood) {}

  /// Finalize
  void finalize();

  /** @name Setters
   */
  ///@{

  /// Set the binary operator for comparing evaluations
  void set_compare(std::function<bool(double, double)> x) { _compare = x; }

  /** Set patience.

      Number of consecutive rejected moves before throwing a
      LocalMaximumReached exception

      \param x Patience

      If x <= 0 then patience is considered infinite, meaning that
      the algorithm will never throw any LocalMaximumReached exception.
  */
  void set_patience(int x) { _patience = x; }

  /// Set incremental evaluation
  void set_incremental_evaluation(bool x) { _incremental_evaluation = x; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
