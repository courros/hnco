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

#ifndef HNCO_MULTIOBJECTIVE_ALGORITHMS_CANDIDATE_SET_H
#define HNCO_MULTIOBJECTIVE_ALGORITHMS_CANDIDATE_SET_H

#include "hnco/multiobjective/functions/function.hh"


namespace hnco {
namespace multiobjective {
namespace algorithm {


/// Candidate set
struct CandidateSet {

  using Function = hnco::multiobjective::function::Function;
  using value_t = hnco::multiobjective::function::value_t;

  /// Bit vectors
  std::vector<bit_vector_t> bvs;

  /// Values
  std::vector<value_t> values;

  /// Pareto fronts
  std::vector<int> pareto_fronts;

  /// Indices
  std::vector<int> indices;

  /** Constructor.
      \param population_size Population size
      \param n Size of bit vectors
      \param num_objectives Number of objectives
  */
  CandidateSet(int population_size, int n, int num_objectives)
    : bvs(population_size, bit_vector_t(n))
    , values(population_size, value_t(num_objectives))
    , pareto_fronts(population_size)
    , indices(population_size)
  {
    if (population_size <= 0)
      throw std::runtime_error("multiobjective::CandidateSet::CandidateSet: population_size must be positive");
    if (n <= 0)
      throw std::runtime_error("multiobjective::CandidateSet::CandidateSet: bit vector size must be positive");
    if (num_objectives <= 0)
      throw std::runtime_error("multiobjective::CandidateSet::CandidateSet: num_objectives size must be positive");
  }

  /// Get the size of the candidate set
  int size() const { return bvs.size(); }

  /// Sample a random candidate set
  void random();

  /** Evaluate a candidate set.

   */
  void evaluate(Function *function);

  /** Evaluate a candidate set in parallel.

   */
  void evaluate_in_parallel(const std::vector<Function *>& functions);

};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
