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

#include <assert.h>

#include <utility>              // std::swap

#include "population.hh"


namespace hnco {
namespace algorithm {


class CommaSelection
{
  Population& _parents;
  Population& _children;

public:

  /** Constructor.

   */
  CommaSelection(Population& parents, Population& children)
    : _parents(parents)
    , _children(children)
  {
    require(children.size() >= parents.size(),
            "CommaSelection::CommaSelection: Children population size must be greater than or equal to parents population size");
  }

  void select() {
    _children.partial_sort(_parents.size());
    for (int i = 0; i < _parents.size(); i++) {
      _parents.values[i] = _children.get_best_value(i);
      std::swap(_parents.bvs[i], _children.get_best_bv(i));
    }
    perm_identity(_parents.permutation);
  }

};


class PlusSelection
{
  Population& _parents;
  Population& _children;
  Population _pool;

public:

  /** Constructor.

   */
  PlusSelection(Population& parents, Population& children)
    : _parents(parents)
    , _children(children)
    , _pool(parents.size() + children.size(), parents.get_bv_size())
  {
    require(children.get_bv_size() == parents.get_bv_size(),
            "PlusSelection::PlusSelection: Bit vectors in parents and children populations must have the same size");
  }

  void select() {
    for (int i = 0; i < _parents.size(); i++) {
      _pool.values[i] = _parents.values[i];
      std::swap(_pool.bvs[i], _parents.bvs[i]);
    }
    int offset = _parents.size();
    for (int i = 0; i < _children.size(); i++) {
      _pool.values[offset + i] = _children.values[i];
      std::swap(_pool.bvs[offset + i], _children.bvs[i]);
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
