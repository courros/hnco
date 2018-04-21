/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#include "hnco/algorithms/algorithm.hh"
#include "hnco/neighborhoods/neighborhood.hh"
#include "hnco/random.hh"

#include "tournament-selection.hh"
#include "crossover.hh"


namespace hnco {
namespace algorithm {


  /** Genetic algorithm.

      - Tournament selection for reproduction
      - Uniform crossover
      - Mutation
      - (mu, mu) selection (offspring population replaces parent population)

      Reference:

      J. H. Holland. 1975. Adaptation in natural and artificial
      systems. University of Michigan Press, Ann Arbor.

  */
  class GeneticAlgorithm:
    public IterativeAlgorithm {

    /// Parents
    TournamentSelection _parents;

    /// Offsprings
    TournamentSelection _offsprings;

    /// Mutation operator
    neighborhood::BernoulliProcess _mutation;

    /// Do crossover
    std::bernoulli_distribution _do_crossover;

    /// Uniform crossover
    UniformCrossover _crossover;

    /** @name Parameters
     */
    ///@{

    /// Mutation probability
    double _mutation_probability;

    /// Crossover probability
    double _crossover_probability = 0.5;

    /// Tournament size
    int _tournament_size = 10;

    /// Allow stay
    bool _allow_stay = false;

    ///@}

    /// Single iteration
    void iterate();

  public:

    /** Constructor.

        \param n Size of bit vectors
        \param mu %Population size
    */
    GeneticAlgorithm(int n, int mu):
      IterativeAlgorithm(n),
      _parents(mu, n),
      _offsprings(mu, n),
      _mutation(n),
      _mutation_probability(1 / double(n)) {}

    /// Initialization
    void init();

    /** @name Setters
     */
    ///@{

    /// Set the mutation probability
    void set_mutation_probability(double x) { _mutation_probability = x; }

    /// Set the crossover probability
    void set_crossover_probability(double x) { _crossover_probability = x; }

    /// Set the tournament size
    void set_tournament_size(int x) { _tournament_size = x; }

    /** Set the flag _allow_stay.

        In case no mutation occurs allow the current bit vector to
        stay unchanged.
    */
    void set_allow_stay(bool x) { _allow_stay = x; }

    ///@}

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
