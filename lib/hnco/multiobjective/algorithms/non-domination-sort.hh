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

#ifndef HNCO_MULTIOBJECTIVE_ALGORITHMS_NON_DOMINATION_SORT_H
#define HNCO_MULTIOBJECTIVE_ALGORITHMS_NON_DOMINATION_SORT_H

#include <assert.h>

#include <unordered_set>
#include <numeric>              // std::iota

#include "hnco/multiobjective/functions/value.hh"
#include "hnco/util.hh"         // hnco::is_in_range

#include "candidate-set.hh"


namespace hnco {
namespace multiobjective {
namespace algorithm {


/** Non domination sort from the NSGA-II paper.

 */
class Nsga2NonDominationSort {

  /// Candidate set
  CandidateSet& _candidate_set;

  /// Pool of values to consider for inclusion in the Pareto front
  std::vector<int> _pool;

  /// Next pool of values
  std::vector<int> _next_pool;

  /// Non dominated values
  std::unordered_set<int> _non_dominated;

  /** Dominated values.

      To be removed from the non dominated ones.
  */
  std::unordered_set<int> _dominated;

  /** Check that a value is non dominated.

      Check that no value in the non dominated set dominates the
      considered value.

      \param i Index of the value
  */
  bool is_non_dominated(int i) {
    for (auto j : _non_dominated) {
      if (function::dominates(_candidate_set.values[j], _candidate_set.values[i]))
        return false;
    }
    return true;
  }

public:

  /// Constructor
  Nsga2NonDominationSort(CandidateSet& candidate_set)
    : _candidate_set(candidate_set)
  {}

  /// Sort
  void sort() {

    _next_pool.reserve(_candidate_set.size());

    _pool.resize(_candidate_set.size());
    std::iota(_pool.begin(), _pool.end(), 0);

    int front = 0;

    while (!_pool.empty()) {
      _next_pool.clear();
      _non_dominated.clear();
      for (auto i : _pool) {
        if (is_non_dominated(i)) {
          _dominated.clear();
          for (auto j : _non_dominated) {
            if (function::dominates(_candidate_set.values[i], _candidate_set.values[j])) {
              _dominated.insert(j);
            }
          }
          for (auto j : _dominated) {
            assert(_non_dominated.count(j) == 1);
            _non_dominated.erase(j);
            _next_pool.push_back(j);
          }
          assert(_non_dominated.count(i) == 0);
          _non_dominated.insert(i);
        } else {
          _next_pool.push_back(i);
        }
      }
      for (auto i : _non_dominated) {
        _candidate_set.pareto_fronts[i] = front;
      }
      front++;
      std::swap(_pool, _next_pool);
    }

  }

};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
