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

#ifndef HNCO_ALGORITHMS_EA_GENETIC_ALGORITHM_H
#define HNCO_ALGORITHMS_EA_GENETIC_ALGORITHM_H

#include <random>

#include "hnco/algorithms/iterative-algorithm.hh"
#include "hnco/algorithms/random-selection.hh"
#include "hnco/algorithms/selection.hh"
#include "hnco/neighborhoods/neighborhood.hh"
#include "hnco/random.hh"

#include "crossover.hh"


namespace hnco {
namespace algorithm {


/** Genetic algorithm.

    - Tournament selection for reproduction
    - Uniform crossover
    - Standard bit mutation
    - (mu, mu) selection (offspring population replaces parent population)

    Reference:

    J. H. Holland. 1975. Adaptation in natural and artificial
    systems. University of Michigan Press, Ann Arbor.

*/
class GeneticAlgorithm:
    public IterativeAlgorithm {

protected:

  /// Parents
  Population _parents;

  /// Offsprings
  Population _offsprings;

  /// Comma selection
  CommaSelection _comma_selection;

  /// Tournament selection
  TournamentSelection _tournament_selection;

  /// Mutation operator
  neighborhood::StandardBitMutation _mutation;

  /// Do crossover
  std::bernoulli_distribution _do_crossover;

  /// Uniform crossover
  UniformCrossover _crossover;

  /** @name Parameters
   */
  ///@{

  /// Mutation rate
  double _mutation_rate;

  /// Crossover probability
  double _crossover_probability = 0.5;

  /// Tournament size
  int _tournament_size = 10;

  /// Allow no mutation
  bool _allow_no_mutation = false;

  ///@}

  /** @name Loop
   */
  ///@{

  /// Initialize
  void init() override;

  /// Single iteration
  void iterate() override;

  ///@}

public:

  /** Constructor.

      \param n Size of bit vectors
      \param mu %Population size
  */
  GeneticAlgorithm(int n, int mu):
    IterativeAlgorithm(n),
    _parents(mu, n),
    _offsprings(mu, n),
    _comma_selection(_parents, _offsprings),
    _tournament_selection(_parents),
    _mutation(n),
    _mutation_rate(1 / double(n)) {}

  /** @name Setters
   */
  ///@{

  /// Set the mutation rate
  void set_mutation_rate(double p) { _mutation_rate = p; }

  /// Set the crossover probability
  void set_crossover_probability(double p) { _crossover_probability = p; }

  /// Set the tournament size
  void set_tournament_size(int n) { _tournament_size = n; }

  /// Set the flag _allow_no_mutation
  void set_allow_no_mutation(bool b) { _allow_no_mutation = b; }

  ///@}

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
