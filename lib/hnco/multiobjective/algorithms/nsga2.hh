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

#include "hnco/algorithms/ea/crossover.hh"    // hnco::algorithm::UniformCrossover
#include "hnco/neighborhoods/neighborhood.hh" // hnco::neighborhood::StandardBitMutation

#include "iterative-algorithm.hh"
#include "non-domination-sort.hh"
#include "random-selection.hh"


namespace hnco {
namespace multiobjective {
namespace algorithm {


/// NSGA-II
class Nsga2: public IterativeAlgorithm {

protected:

  /// Parent population
  CandidateSet _parents;

  /// Offspring population
  CandidateSet _offsprings;

  /// Augmented population
  CandidateSet _augmented_population;

  /// Selection
  TournamentSelection _selection;

  /// Mutation operator
  neighborhood::StandardBitMutation _mutation;

  /// Do crossover
  std::bernoulli_distribution _do_crossover;

  /// Uniform crossover
  hnco::algorithm::UniformCrossover _crossover;

  /// Non domination sort
  Nsga2NonDominationSort _non_domination_sort;

  /// Crowding distance
  std::vector<double> _crowding_distance;

  /** @name Parameters
   */
  ///@{

  /// Tournament size
  int _tournament_size = 10;

  /// Mutation rate
  double _mutation_rate;

  /// Allow no mutation
  bool _allow_no_mutation = false;

  /// Crossover probability
  double _crossover_probability = 0.5;

  ///@}

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
  */
  Nsga2(int n, int num_objectives, int population_size)
    : IterativeAlgorithm(n, num_objectives)
    , _parents(population_size, n, num_objectives)
    , _offsprings(population_size, n, num_objectives)
    , _augmented_population(2 * population_size, n, num_objectives)
    , _selection(_parents)
    , _mutation(n)
    , _non_domination_sort(_augmented_population)
    , _crowding_distance(population_size)
    , _mutation_rate(1 / double(n))
  {}

  /// Get solutions
  const CandidateSet& get_solutions() { return _parents; }

  /** @name Setters
   */
  ///@{

  /// Set the tournament size
  void set_tournament_size(int n) { _tournament_size = n; }

  /// Set the mutation rate
  void set_mutation_rate(double p) { _mutation_rate = p; }

  /// Set the flag _allow_no_mutation
  void set_allow_no_mutation(bool b) { _allow_no_mutation = b; }

  /// Set the crossover probability
  void set_crossover_probability(double p) { _crossover_probability = p; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif