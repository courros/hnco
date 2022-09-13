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

#ifndef HNCO_ALGORITHMS_SELECTION_H
#define HNCO_ALGORITHMS_SELECTION_H

#include <utility>              // std::swap

#include "hnco/util.hh"         // hnco::require
#include "hnco/algorithms/population.hh"


namespace hnco {
namespace algorithm {


/**
 * Comma selection. Used as selection for replacement in evolutionary
 * algorithms.
 */
class CommaSelection
{
  /// Parent population
  Population& _parents;

  /// Offspring population
  Population& _offsprings;

public:

  /**
   * Constructor.
   * @param parents Parent population
   * @param offsprings Offspring population
   */
  CommaSelection(Population& parents, Population& offsprings)
    : _parents(parents)
    , _offsprings(offsprings)
  {
    require(offsprings.size() >= parents.size(),
            "CommaSelection::CommaSelection: Offspring population size must be greater than or equal to parent population size");
  }

  /// Apply selection
  void select() {
    _offsprings.partial_sort(_parents.size());
    for (int i = 0; i < _parents.size(); i++) {
      _parents.values[i] = _offsprings.get_best_value(i);
      std::swap(_parents.bvs[i], _offsprings.get_best_bv(i));
    }
    perm_identity(_parents.permutation);
  }

};


/**
 * Plus selection. Used as selection for replacement in evolutionary
 * algorithms.
 */
class PlusSelection
{
  Population& _parents;
  Population& _offsprings;
  Population _pool;

public:

  /**
   * Constructor.
   * @param parents Parent population
   * @param offsprings Offspring population
   */
  PlusSelection(Population& parents, Population& offsprings)
    : _parents(parents)
    , _offsprings(offsprings)
    , _pool(parents.size() + offsprings.size(), parents.get_bv_size())
  {
    require(offsprings.get_bv_size() == parents.get_bv_size(),
            "PlusSelection::PlusSelection: Bit vectors in parent and offspring populations must have the same size");
  }

  /// Apply selection
  void select() {
    for (int i = 0; i < _parents.size(); i++) {
      _pool.values[i] = _parents.values[i];
      std::swap(_pool.bvs[i], _parents.bvs[i]);
    }
    int offset = _parents.size();
    for (int i = 0; i < _offsprings.size(); i++) {
      _pool.values[offset + i] = _offsprings.values[i];
      std::swap(_pool.bvs[offset + i], _offsprings.bvs[i]);
    }
    _pool.partial_sort(_parents.size());
    for (int i = 0; i < _parents.size(); i++) {
      _parents.values[i] = _pool.get_best_value(i);
      std::swap(_parents.bvs[i], _pool.get_best_bv(i));
    }
    perm_identity(_parents.permutation);
  }

};


}
}


#endif
