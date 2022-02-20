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

#ifndef HNCO_MULTIOBJECTIVE_ALGORITHMS_RANDOM_SELECTION_H
#define HNCO_MULTIOBJECTIVE_ALGORITHMS_RANDOM_SELECTION_H

#include "hnco/bit-vector.hh"
#include "hnco/permutation.hh"


namespace hnco {
namespace multiobjective {
namespace algorithm {


/** Tournament selection.

    Implement tournament selection without replacement as explained in
    the reference:

    Goldberg, Korb, and Deb, "Messy genetic algorithms: Motivation,
    analysis, and first results", Complex systems, 1989.

    https://www.complex-systems.com/abstracts/v03_i05_a05/

*/
template<typename T>
class TournamentSelection {

  /// Population
  const std::vector<bit_vector_t>& _population;

  /// Values
  const std::vector<T>& _values;

  /// Permutation
  hnco::permutation_t _permutation;

  /// Beginning of the slice of permutation used in a tournament round
  int _start;

  /// End of the slice of permutation used in a tournament round
  int _stop;

  /** @name Parameters
   */
  ///@{

  /// Tournament size
  int _tournament_size = 2;

  ///@}

public:

  /// Constructor
  TournamentSelection(const std::vector<bit_vector_t>& population, const std::vector<T>& values)
    : _population(population)
    , _values(values)
    , _permutation(population.size())
  {
    assert(is_in_range(_tournament_size, 2, population.size()));
  }

  /// Initialize
  void init() {
    perm_random(_permutation);
    _start = 0;
    _stop = _tournament_size;
  }

  /// Select a bit vector
  const bit_vector_t& select() {
    assert(is_in_range(_stop, 2, _population.size()));

    int winner = _start;
    T best_value = _values[_start];
    for (int challenger = _start + 1; challenger < _stop; challenger++) {
      T value = _values[challenger];
      if (value > best_value) {
        winner = challenger;
        best_value = value;
      }
    }

    _start = _stop;
    _stop += _tournament_size;
    if (_stop > int(_population.size()))
      init();

    return _population[winner];
  }

  /** @name Setters
   */
  ///@{

  /// Set the tournament size
  void set_tournament_size(int n) {
    assert(is_in_range(n, 2, _population.size()));

    _tournament_size = n;
  }

  ///@}

};


} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco


#endif
