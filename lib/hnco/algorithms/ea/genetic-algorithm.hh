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

#ifndef HNCO_ALGORITHMS_EA_GENETIC_ALGORITHM_H
#define HNCO_ALGORITHMS_EA_GENETIC_ALGORITHM_H

#include <random>

#include "hnco/algorithms/algorithm.hh"

#include "tournament-selection.hh"


namespace hnco {
namespace algorithm {


  /** Genetic algorithm.

      - Tournament selection for reproduction
      - Uniform crossover
      - Mutation
      - (mu, mu) selection (offspring population replaces parent population)
  */
  class GeneticAlgorithm:
    public IterativeAlgorithm {

    /// Parents
    TournamentSelection _parents;

    /// Offsprings
    TournamentSelection _offsprings;

    /// Do crossover
    std::bernoulli_distribution _do_crossover;

    /// Do mutation
    std::bernoulli_distribution _do_mutation;

    /// Single iteration
    void iterate();

  public:

    /// Constructor
    GeneticAlgorithm(int n, int mu):
      IterativeAlgorithm(n),
      _parents(mu, n),
      _offsprings(mu, n),
      _mutation_probability(1 / double(n)) {}

    /// Initialization
    void init();

    /** @name Parameters
     */
    ///@{

    /// Mutation probability
    double _mutation_probability;

    /// Crossover probability
    double _crossover_probability = 0.5;

    /// Tournament size
    int _tournament_size = 10;

    ///@}

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
