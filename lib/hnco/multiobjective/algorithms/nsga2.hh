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

#ifndef HNCO_MULTIOBJECTIVE_ALGORITHMS_NSGA2_H
#define HNCO_MULTIOBJECTIVE_ALGORITHMS_NSGA2_H

#include "iterative-algorithm.hh"
#include "non-domination-sort.hh"


namespace hnco {
namespace multiobjective {
namespace algorithm {


/// NSGA-II
class Nsga2: public IterativeAlgorithm {

protected:

  /// Parent population
  CandidateSet _population;

  /// Augmented population
  CandidateSet _augmented_population;

  /// Non domination sort
  NonDominationSort *_non_domination_sort;

  /** @name Loop
   */
  ///@{

  /// Initialize
  void init() override;

  /// Single iteration
  void iterate();

  /// Log
  void log() override {}

  ///@}


public:

  /** Constructor.

      \param n Size of bit vectors
      \param num_objectives Number of objectives
      \param population_size Population size
      \param non_domination_sort Non domination sort operator
  */
  Nsga2(int n, int num_objectives, int population_size, NonDominationSort *non_domination_sort)
    : _population(population_size, n, num_objectives)
    , _augmented_population(2 * population_size, n, num_objectives)
    , _non_domination_sort(non_domination_sort)
  {
    assert(non_domination_sort);
  }

  /// Get solutions
  const CandidateSet& get_solutions() { return _population; }

};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
