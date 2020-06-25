/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

#include <functional>           // std::function
#include <algorithm>            // std::sort, std::shuffle

#include "hnco/functions/function.hh"
#include "hnco/random.hh"


namespace hnco {
namespace algorithm {


/// Population
class Population {

protected:

  /// Index-value type
  typedef std::pair<int, double> index_value_t;

  /// Bit vectors
  std::vector<bit_vector_t> _bvs;

  /** Lookup table.

      Let p be of type std::pair<int, double>. Then p.first is the
      bv index in the unsorted population whereas p.second is the bv
      value.
  */
  std::vector<index_value_t> _lookup;

  /// Binary operator for comparing index-value pairs
  std::function<bool(const index_value_t&, const index_value_t&)> _compare_index_value =
    [](const index_value_t& a, const index_value_t& b) { return a.second > b.second; };

public:

  /// Constructor
  Population(int population_size, int n):
    _bvs(population_size, bit_vector_t(n)),
    _lookup(population_size) {}

  /// Size
  int size() const { return _bvs.size(); }

  /// Initialize the population with random bit vectors
  void random();


  /** @name Get bit vectors for non const populations
   */
  ///@{

  /// Get a bit vector
  bit_vector_t& get_bv(int i) { return _bvs[i]; }

  /** Get best bit vector.

      \pre The population must be sorted.
  */
  bit_vector_t& get_best_bv() { return _bvs[_lookup[0].first]; }

  /** Get best bit vector.

      \param i Index in the sorted population

      \pre The population must be sorted.
  */
  bit_vector_t& get_best_bv(int i) { return _bvs[_lookup[i].first]; }

  /** Get worst bit vector.

      \param i Index in the sorted population

      \pre The population must be sorted.
  */
  bit_vector_t& get_worst_bv(int i) { return get_best_bv(_bvs.size() - 1 - i); }

  ///@}


  /** @name Get bit vectors for const populations
   */
  ///@{

  /// Get a bit vector
  const bit_vector_t& get_bv(int i) const { return _bvs[i]; }

  /** Get best bit vector.

      \pre The population must be sorted.
  */
  const bit_vector_t& get_best_bv() const { return _bvs[_lookup[0].first]; }

  /** Get best bit vector.

      \param i Index in the sorted population

      \pre The population must be sorted.
  */
  const bit_vector_t& get_best_bv(int i) const { return _bvs[_lookup[i].first]; }

  /** Get worst bit vector.

      \param i Index in the sorted population

      \pre The population must be sorted.
  */
  const bit_vector_t& get_worst_bv(int i) const { return get_best_bv(_bvs.size() - 1 - i); }

  ///@}


  /** @name Get sorted values
   */
  ///@{

  /** Get best value.

      \param i Index in the sorted population

      \pre The population must be sorted.
  */
  double get_best_value(int i) const { return _lookup[i].second; }

  /** Get best value.

      \pre The population must be sorted.
  */
  double get_best_value() const { return _lookup[0].second; }

  ///@}


  /** @name Evaluation and sorting
   */
  ///@{

  /// Evaluate the population
  void evaluate(function::Function *function);

  /// Evaluate the population in parallel
  void evaluate_in_parallel(const std::vector<function::Function *>& functions);

  /// Sort the lookup table
  void sort() { std::sort(_lookup.begin(), _lookup.end(), _compare_index_value); }

  /// Partially sort the lookup table
  void partial_sort(int selection_size) {
    assert(selection_size > 0);
    std::partial_sort(_lookup.begin(), _lookup.begin() + selection_size, _lookup.end(), _compare_index_value);
  }

  /// Shuffle the lookup table
  void shuffle() { std::shuffle(_lookup.begin(), _lookup.end(), random::Generator::engine); }

  ///@}


  /** @name Selection
   */
  ///@{

  /** Plus selection.

      Implemented with a copy.

      \pre Both populations must be completely sorted.

      \warning The function does not break ties randomly
      (workaround: shuffle parents and offsprings).
  */
  void plus_selection(const Population& offsprings);

  /** Plus selection.

      Implemented with a swap. Should be faster than plus_selection
      with a copy.

      \pre Both populations must be completely sorted.

      \warning The function does not break ties randomly
      (workaround: shuffle parents and offsprings).

      \warning Modifies its argument.
  */
  void plus_selection(Population& offsprings);

  /** Comma selection.

      Implemented with a copy.

      \pre Offspring population must be partially sorted.

      \warning The function does not break ties randomly
      (workaround: shuffle offsprings).
  */
  void comma_selection(const Population& offsprings);

  /** Comma selection.

      Implemented with a swap. Should be faster than comma_selection
      with a copy.

      \pre Offspring population must be partially sorted.

      \warning The function does not break ties randomly
      (workaround: shuffle offsprings).

      \warning Modifies its argument.
  */
  void comma_selection(Population& offsprings);

  ///@}

};


}
}


#endif
