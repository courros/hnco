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

#ifndef HNCO_ALGORITHMS_POPULATION_H
#define HNCO_ALGORITHMS_POPULATION_H

#include <assert.h>

#include <algorithm>            // std::sort

#include "hnco/bit-vector.hh"
#include "hnco/functions/function.hh"
#include "hnco/permutation.hh"
#include "hnco/random.hh"       // random::Generator::engine
#include "hnco/util.hh"         // hnco::require


namespace hnco {
namespace algorithm {


/// %Population
struct Population {

  /// %Function type
  using Function = hnco::function::Function;

  /// Bit vectors
  std::vector<bit_vector_t> bvs;

  /// Values
  std::vector<double> values;

  /// Permutation
  hnco::permutation_t permutation;

  /** Constructor.

      \param population_size Population size
      \param n Bit vector size
  */
  Population(int population_size, int n)
    : bvs(population_size, bit_vector_t(n))
    , values(population_size)
    , permutation(population_size)
  {
    require(population_size > 0, "Population::Population: population_size must be positive");
    require(n > 0, "Population::Population: bit vector size must be positive");

    perm_identity(permutation);
  }

  /// Size
  int size() const { return bvs.size(); }

  /// Get bit vector size
  int get_bv_size() const { return bvs[0].size(); }

  /// Sample a random population
  void random();


  /** @name Get sorted bit vectors
   */
  ///@{

  /** Get best bit vector.

      \pre The population must be sorted.
  */
  bit_vector_t& get_best_bv() { return bvs[permutation[0]]; }

  /** Get best bit vector.

      \param i Index in the sorted population

      \pre The population must be sorted.
  */
  bit_vector_t& get_best_bv(int i) { return bvs[permutation[i]]; }

  /** Get worst bit vector.

      \param i Index in the sorted population

      \pre The population must be sorted.
  */
  bit_vector_t& get_worst_bv(int i) { return get_best_bv(bvs.size() - 1 - i); }

  ///@}


  /** @name Get sorted values
   */
  ///@{

  /** Get best value.

      \pre The population must be sorted.
  */
  double get_best_value() const { return values[permutation[0]]; }

  /** Get best value.

      \param i Index in the sorted population

      \pre The population must be sorted.
  */
  double get_best_value(int i) const { return values[permutation[i]]; }

  ///@}


  /** @name Evaluation and sorting
   */
  ///@{

  /// Evaluate the population
  void evaluate(Function *function);

  /// Evaluate the population in parallel
  void evaluate_in_parallel(const std::vector<Function *>& functions);

  /** Sort the population

      Only the permutation is sorted using the order defined by i < j
      if value[i] > value[j].

      Before sorting, the permutation is shuffled to break ties
      randomly.
  */
  void sort() {
    auto compare = [this](int i, int j){ return this->values[i] > this->values[j]; };
    perm_shuffle(permutation);
    std::sort(permutation.begin(), permutation.end(), compare);
  }

  /** Partially sort the population

      Only the permutation is sorted using the order defined by i < j
      if value[i] > value[j].

      Before sorting, the permutation is shuffled to break ties
      randomly.

      \param selection_size Sort the best selection_size individuals
  */
  void partial_sort(int selection_size) {
    assert(selection_size > 0);

    auto compare = [this](int i, int j){ return this->values[i] > this->values[j]; };
    perm_shuffle(permutation);
    std::partial_sort(permutation.begin(), permutation.begin() + selection_size, permutation.end(), compare);
  }

  ///@}

};


}
}


#endif
