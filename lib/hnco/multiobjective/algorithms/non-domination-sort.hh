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

#include <unordered_set>
#include <numeric>              // std::iota

#include "hnco/multiobjective/functions/value.hh"

#include "candidate-set.hh"


namespace hnco {
namespace multiobjective {
namespace algorithm {


/** Non domination sort from the NSGA-II paper.

 */
class Nsga2NonDominationSort {

  /// Candidate set
  CandidateSet& _candidate_set;

  std::vector<int> _pool;
  std::vector<int> _next_pool;
  std::unordered_set<int> _non_dominated;
  std::unordered_set<int> _dominated;

  bool is_non_dominated(int i) {
    for (auto j : _non_dominated)
      if (function::dominates(_candidate_set.values[j], _candidate_set.values[i]))
        return false;
    return true;
  }

public:

  /// Constructor
  Nsga2NonDominationSort(CandidateSet& candidate_set)
    : _candidate_set(candidate_set)
  {
    _pool.reserve(candidate_set.size());
    _next_pool.reserve(candidate_set.size());
  }

  /// Sort
  void sort() {
    std::iota(_pool.begin(), _pool.end(), 0);
    while (!_pool.empty()) {
      _non_dominated.clear();
      _next_pool.clear();
      for (auto i : _pool) {
        if (is_non_dominated(i)) {
          _dominated.clear();
          for (auto j : _non_dominated) {
            if (function::dominates(_candidate_set.values[i], _candidate_set.values[j])) {
              _dominated.insert(j);
            }
          }
          _non_dominated.erase(_dominated.begin(), _dominated.end());
          _non_dominated.insert(i);
        } else {
          _next_pool.push_back(i);
        }
      }
      // Update _candidate_set with _non_dominated
      std::swap(_pool, _next_pool);
    }
  }

};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
