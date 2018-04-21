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

#ifndef HNCO_ALGORITHMS_POPULATION_H
#define HNCO_ALGORITHMS_POPULATION_H

#include <functional>           // std::function

#include "hnco/functions/function.hh"
#include "hnco/random.hh"


namespace hnco {
namespace algorithm {


  /// Population
  class Population {

  public:

    /// Index-value type
    typedef std::pair<size_t, double> index_value_t;

    /// Binary operator for comparing index-value pairs
    std::function<bool(const index_value_t&, const index_value_t&)> _operator =
      [](const index_value_t& a, const index_value_t& b) { return a.second > b.second; };

  protected:

    /// Bit vectors
    std::vector<bit_vector_t> _bvs;

    /** Lookup table.

        Let p be of type std::pair<size_t, double>. Then p.first is
        the bv index in the unsorted population whereas p.second is
        the bv value.
    */
    std::vector<index_value_t> _lookup;

  public:

    /// Constructor
    Population(int population_size, int n):
      _bvs(population_size, bit_vector_t(n)),
      _lookup(population_size) {}

    /// Size
    std::size_t size() const { return _bvs.size(); }

    /// Initialize the population with random bit vectors
    void random();

    /// Get a bit vector
    bit_vector_t& get_bv(int i) { return _bvs[i]; }

    /// Get a bit vector
    const bit_vector_t& get_bv(int i) const { return _bvs[i]; }


    /** @name Get sorted bit vectors
     */
    ///@{

    /** Get best bit vector.

        \param i Index in the sorted population

        \pre The population must be sorted.
    */
    const bit_vector_t& get_best_bv(int i) const { return _bvs[_lookup[i].first]; }

    /** Get best bit vector.

        \pre The population must be sorted.
    */
    const bit_vector_t& get_best_bv() const { return _bvs[_lookup[0].first]; }

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
    void eval(function::Function *function);

    /// Parallel evaluation of the population
    void eval(const std::vector<function::Function *>& functions);

    /// Sort the lookup table
    void sort();

    /// Partially sort the lookup table
    void partial_sort(int selection_size);

    ///@}


    /** @name Selection
     */
    ///@{

    /** Plus selection.

        \pre Both populations must be sorted.

        \warning The function does not break ties randomly as it
        should.
    */
    void plus_selection(const Population& offsprings);

    /** Comma selection.

        \pre Offspring population must be sorted.

        \warning The function does not break ties randomly as it
        should.
    */
    void comma_selection(const Population& offsprings);

    ///@}

  };


}
}


#endif
