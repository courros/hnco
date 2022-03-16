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

#include <functional>           // std::less

#include "hnco/algorithms/ea/crossover.hh"    // hnco::algorithm::UniformCrossover
#include "hnco/neighborhoods/neighborhood.hh" // hnco::neighborhood::StandardBitMutation

#include "iterative-algorithm.hh"
#include "pareto-front-computation.hh"
#include "random-selection.hh"


namespace hnco {
namespace multiobjective {
namespace algorithm {


/** Front-distance pair.

    A front-distance pair measures the quality of an individual within
    a population.
*/
struct FrontDistancePair {

  /// Pareto front
  int pareto_front;

  /// Crowding distance
  double crowding_distance;

};

/** Comparison operator for front-distance pairs.

    Favors individuals with smaller Pareto front then greater crowding
    distance.
*/
inline bool operator<(const FrontDistancePair& a, const FrontDistancePair& b)
{
  if (a.pareto_front < b.pareto_front)
    return true;
  if (a.pareto_front == b.pareto_front && a.crowding_distance > b.crowding_distance)
    return true;
  return false;
}

/** NSGA-II.

    NSGA-II is a (mu+mu) evolutionary algorithm for multiobjective
    optimization.

    Deb, Agrawal, Pratap, and Meyarivan, "A Fast Elitist Non-dominated
    Sorting Genetic %Algorithm for Multi-objective Optimization:
    NSGA-II", Parallel Problem Solving from Nature PPSN VI, 2000,
    Springer Berlin Heidelberg.

    https://link.springer.com/chapter/10.1007/3-540-45356-3_83

*/
class Nsga2: public IterativeAlgorithm {

protected:

  /// Parent population
  Population _parents;

  /// Offspring population
  Population _offsprings;

  /// Full population
  Population _full_population;

  /// Solutions
  Population _solutions;

  /// Mutation operator
  neighborhood::StandardBitMutation _mutation;

  /// Do crossover
  std::bernoulli_distribution _do_crossover;

  /// Uniform crossover
  hnco::algorithm::UniformCrossover _crossover;

  /// Pareto front computation
  Nsga2ParetoFrontComputation _pareto_front_computation;

  /// Pareto fronts
  std::vector<int> _pareto_fronts;

  /// Crowding distances
  std::vector<double> _crowding_distances;

  /// Permutation relative to Pareto front
  hnco::permutation_t _permutation;

  /// Front distance pairs
  std::vector<FrontDistancePair> _front_distance_pairs;

  /// Selection by front distance pairs
  TournamentSelection<FrontDistancePair, std::less<FrontDistancePair>> _selection_by_front_distance_pair;

  /** @name Parameters
   */
  ///@{

  /// Tournament size
  int _tournament_size = 2;

  /// Mutation rate
  double _mutation_rate;

  /// Allow no mutation
  bool _allow_no_mutation = false;

  /// Crossover probability
  double _crossover_probability = 0.8;

  ///@}

  /** @name Loop
   */
  ///@{

  /// Initialize
  void init() override;

  /// Single iteration
  void iterate();

  /// Finalize
  void finalize() override;

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
    , _full_population(2 * population_size, n, num_objectives)
    , _solutions(population_size, n, num_objectives)
    , _mutation(n)
    , _pareto_front_computation(_full_population)
    , _pareto_fronts(2 * population_size)
    , _crowding_distances(2 * population_size)
    , _permutation(2 * population_size)
    , _front_distance_pairs(population_size)
    , _selection_by_front_distance_pair(_parents.bvs, _front_distance_pairs)
    , _mutation_rate(1 / double(n))
  {}

  /// Get solutions
  const Population& get_solutions() { return _solutions; }

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
