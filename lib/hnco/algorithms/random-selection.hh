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

#ifndef HNCO_ALGORITHMS_RANDOM_SELECTION_H
#define HNCO_ALGORITHMS_RANDOM_SELECTION_H

#include "hnco/algorithms/population.hh"
#include "hnco/functions/function.hh"
#include "hnco/random.hh"


namespace hnco {
namespace algorithm {


/// Random selection
class RandomSelection: public Population
{

public:

  /** Constructor.

      \param population_size %Population size
      \param n Bit vector size
  */
  RandomSelection(int population_size, int n)
    : Population(population_size, n)
  {}

  /// Initialize
  virtual void init() {}

  /// Select an individual in the population
  virtual const bit_vector_t& select() = 0;

};


/// Uniform selection
class UniformSelection: public RandomSelection
{

  /// Random index
  std::uniform_int_distribution<int> _choose_individual;

public:

  /** Constructor.

      \param population_size %Population size
      \param n Bit vector size
  */
  UniformSelection(int population_size, int n):
    RandomSelection(population_size, n),
    _choose_individual(0, population_size - 1) {}

  /// Select an individual in the population
  const bit_vector_t& select() override;

};


/// Tournament selection
class TournamentSelection: public RandomSelection
{

  /// Random index
  std::uniform_int_distribution<int> _choose_individual;

  /** @name Parameters
   */
  ///@{

  /// Tournament size
  int _tournament_size = 10;

  ///@}

public:

  /** Constructor.

      \param population_size %Population size
      \param n Bit vector size
  */
  TournamentSelection(int population_size, int n):
    RandomSelection(population_size, n),
    _choose_individual(0, population_size - 1) {}
  
  /** Select an individual in the population.

      The selection only requires that the population be evaluated,
      not necessarily sorted.

      \pre The population must be evaluated.
  */
  const bit_vector_t& select() override;

  /** @name Setters
   */
  ///@{

  /// Set the tournament size
  void set_tournament_size(int x) { _tournament_size = x; }

  ///@}

};


}
}


#endif
