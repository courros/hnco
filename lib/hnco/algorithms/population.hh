/* Copyright (C) 2016 Arnaud Berny

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

#include "hnco/functions/function.hh"
#include "hnco/random.hh"


namespace hnco {
namespace algorithm {


  /// Population
  class Population
  {

  public:

  /// Evaluation of a bit vector
  struct Evaluation {

    /// Index of the bit vector in the population
    size_t index;

    /// Value
    double value;

  };

  protected:

    /// Bit vectors
    std::vector<bit_vector_t> _population;

    /// Lookup table
    std::vector<Evaluation> _lookup;

  public:

    /// Constructor
    Population(int population_size, int n):
      _population(population_size, bit_vector_t(n)),
      _lookup(population_size) {}

    /// Size
    std::size_t size() const { return _population.size(); }

    /** Get best bit vector.

        \param i Index in the sorted population

        \pre Population must be sorted.
    */
    const bit_vector_t& get_best_bv(int i) const { return _population[_lookup[i].index]; }

    /** Get best bit vector.

        \param i Index in the sorted population
        \param p Population

        \pre p must be sorted.
    */
    const bit_vector_t& get_best_bv(int i, const Population& p) const { return _population[p._lookup[i].index]; }

    /** Get best bit vector.

        \pre Population must be sorted.
    */
    const bit_vector_t& get_best_bv() const { return _population[_lookup[0].index]; }

    /** Get best bit vector.

        \param p Population

        \pre p must be sorted.
    */
    const bit_vector_t& get_best_bv(const Population& p) const { return _population[p._lookup[0].index]; }

    /** Get best index.

        \param i Index in the sorted population

        \return Index in the unsorted population

        \pre Population must be sorted.
    */
    double get_best_index(int i) const { return _lookup[i].index; }

    /** Get best index.

        \return Index in the unsorted population

        \pre Population must be sorted.
    */
    double get_best_index() const { return _lookup[0].index; }

    /** Get best value.

        \param i Index in the sorted population

        \pre Population must be sorted.
    */
    double get_best_value(int i) const { return _lookup[i].value; }

    /** Get best value.

        \pre Population must be sorted.
    */
    double get_best_value() const { return _lookup[0].value; }

    /** Get worst bit vector.

        \param i Index in the sorted population

        \pre Population must be sorted.
    */
    const bit_vector_t& get_worst_bv(int i) const { return get_best_bv(_population.size() - 1 - i); }

    /// Initialize the population with random bit vectors
    void random();

    /// Get a bit vector
    bit_vector_t& get_bv(int i) { return _population[i]; }

    /// Evaluate the population
    void eval(function::Function *function);

    /// Sort the lookup table
    void sort();

    /// Partially sort the lookup table
    void partial_sort(int selection_size);

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

  };


  /// Population with tournament selection
  class TournamentSelection:
    public Population
  {

    /// Random index
    std::uniform_int_distribution<int> _choose_individual;

  public:

    /// Constructor
    TournamentSelection(int population_size, int n):
      Population(population_size, n),
      _choose_individual(0, population_size - 1) {}

    /** Selection.

        The selection only requires that the population be evaluated,
        not necessarily sorted.

        \pre The population must be evaluated.
    */
    const bit_vector_t& select();

    /// Tournament size
    int _tournament_size = 10;

  };

}
}


#endif
