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

#ifndef HNCO_TEST_NON_DOMINATION_SORT_H
#define HNCO_TEST_NON_DOMINATION_SORT_H

#include "hnco/random.hh"

using namespace hnco::multiobjective::algorithm;
using namespace hnco::multiobjective::function;
using namespace hnco::random;
using namespace hnco;


/** Check non domination sort.

    Check that, for all values values[a] and values[b] in the same
    pareto front, neither values[a] dominates values[b] nor values[b]
    dominates values[a].

    Also check that for all values values[a] and values[b] such that
    pareto_fronts[a] < pareto_fronts[b] there exists a value values[i]
    in the same pareto front as values[a] such that values[i]
    dominates values[b].

    Also check that for all values values[a] and values[b] such that
    pareto_fronts[a] > pareto_fronts[b] there exists a value values[i]
    in the same pareto front as values[b] such that values[i]
    dominates values[a].

*/
template<class NonDominationSort>
bool check()
{
  std::uniform_int_distribution<int> dist_population_size(1, 100);
  std::uniform_int_distribution<int> dist_bv_size(1, 100);
  std::uniform_int_distribution<int> dist_num_objectives(2, 10);

  for (int i = 0; i < 10; i++) {
    const int population_size   = dist_population_size(Generator::engine);
    const int bv_size           = dist_bv_size(Generator::engine);
    const int num_objectives    = dist_num_objectives(Generator::engine);

    CandidateSet candidates(population_size, bv_size, num_objectives);
    for (auto& v : candidates.values) {
      for (auto& x : v) {
        x = Generator::uniform();
      }
    }

    NonDominationSort non_domination_sort(candidates);
    non_domination_sort.sort();

    std::uniform_int_distribution<int> dist_index(0, population_size - 1);

    for (int j = 0; j < 100; j++) {
      const int a = dist_index(Generator::engine);
      const int b = dist_index(Generator::engine);
      if (a == b) continue;

      const int pa = candidates.pareto_fronts[a];
      const int pb = candidates.pareto_fronts[b];

      if (pa == pb) {
        if (dominates(candidates.values[a], candidates.values[b]))
          return false;
        if (dominates(candidates.values[b], candidates.values[a]))
          return false;
      } else if (pa < pb) {
        bool found = false;
        for (int i = 0; i < candidates.size(); i++) {
          if (candidates.pareto_fronts[i] == pa && dominates(candidates.values[i], candidates.values[b])) {
            found = true;
            break;
          }
        }
        if (!found)
          return false;
      } else {
        assert(pb < pa);
        bool found = false;
        for (int i = 0; i < candidates.size(); i++) {
          if (candidates.pareto_fronts[i] == pb && dominates(candidates.values[i], candidates.values[a])) {
            found = true;
            break;
          }
        }
        if (!found)
          return false;
      }

    }
  }

  return true;
}


#endif
