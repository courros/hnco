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

#ifndef HNCO_MULTIOBJECTIVE_ALGORITHMS_POPULATION_H
#define HNCO_MULTIOBJECTIVE_ALGORITHMS_POPULATION_H

#include <assert.h>

#include "hnco/multiobjective/functions/function.hh"
#include "hnco/util.hh"         // hnco::ensure


namespace hnco {
namespace multiobjective {
namespace algorithm {


/// %Population
struct Population {

  /// %Function type
  using Function = hnco::multiobjective::function::Function;

  /// Value type
  using value_t = hnco::multiobjective::function::value_t;

  /// Bit vectors
  std::vector<bit_vector_t> bvs;

  /// Values
  std::vector<value_t> values;

  /// Default constructor
  Population() = default;

  /**
   * Constructor.
   * @param population_size %Population size
   * @param n Size of bit vectors
   * @param num_objectives Number of objectives
   */
  Population(int population_size, int n, int num_objectives)
    : bvs(population_size, bit_vector_t(n))
    , values(population_size, value_t(num_objectives))
  {
    ensure(population_size > 0,
           "multiobjective::Population::Population: population size must be positive");
    ensure(n > 0,
           "multiobjective::Population::Population: bit vector size must be positive");
    ensure(num_objectives > 0,
           "multiobjective::Population::Population: num_objectives must be positive");
  }

  /// Get the population size
  int get_size() const { return bvs.size(); }

  /**
   * Resize the population.
   * @param population_size %Population size
   * @param n Size of bit vectors
   * @param num_objectives Number of objectives
   */
  void resize(int population_size, int n, int num_objectives) {
    assert(population_size > 0);

    const int old_size = get_size();
    bvs.resize(population_size);
    values.resize(population_size);
    if (population_size > old_size) {
      for (int i = old_size; i < population_size; i++) {
        bvs[i] = bit_vector_t(n);
        values[i] = value_t(num_objectives);
      }
    }
  }

  /**
   * Shrink the population.
   * @param population_size %Population size
   * @pre population_size < size()
   */
  void shrink(int population_size) {
    assert(population_size > 0);
    assert(population_size < get_size());

    bvs.resize(population_size);
    values.resize(population_size);
  }

  /// Sample a random population
  void random();

  /**
   * Evaluate a population.
   */
  void evaluate(Function *function);

  /**
   * Evaluate a population in parallel.
   */
  void evaluate_in_parallel(const std::vector<Function *>& functions);

};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
