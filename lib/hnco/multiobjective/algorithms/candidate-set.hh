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


namespace hnco {
namespace multiobjective {
namespace algorithm {

using Function = hnco::multiobjective::function::Function;

/// Candidate set
struct CandidateSet {

  /// Bit vectors
  std::vector<bit_vector_t> bvs;

  /// Values
  std::vector<value_t> values;

  /// Ranks
  std::vector<int> ranks;

  /** Constructor.
      \param population_size Population size
      \param n Size of bit vectors
      \param num_objectives Number of objectives
  */
  CandidateSet(int population_size, int n, int num_objectives)
    : bvs(population_size, bit_vector_t(n))
    , values(population_size, value_t(num_objectives))
    , ranks(population_size)
  {}

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

  /** Check whether the candidate set is non dominated.

      \return True if the candidate set is non dominated
  */
  bool is_non_dominated_set() const;

};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
