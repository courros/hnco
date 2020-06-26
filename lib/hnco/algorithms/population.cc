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

#include <assert.h>
#include <omp.h>                // omp_get_thread_num

#include "hnco/util.hh"         // hnco::is_in_range

#include "population.hh"


using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


void
Population::random()
{
  for (size_t i = 0; i < _bvs.size(); i++)
    bv_random(_bvs[i]);
}


void
Population::evaluate(Function *function)
{
  assert(function);
  assert(_bvs.size() == _lookup.size());

  for (size_t i = 0; i < _bvs.size(); i++) {
    _lookup[i].first = i;
    _lookup[i].second = function->evaluate(_bvs[i]);
  }

}


void
Population::evaluate_in_parallel(const std::vector<Function *>& fns)
{
  assert(_bvs.size() == _lookup.size());
  assert(fns.size() >= 1);

#pragma omp parallel for
  for (size_t i = 0; i < _bvs.size(); i++) {
    int k = omp_get_thread_num();
    assert(is_in_range(k, int(fns.size())));
    _lookup[i].first = i;
    _lookup[i].second = fns[k]->evaluate_safely(_bvs[i]);
  }

  for (size_t i = 0; i < _bvs.size(); i++)
    fns[0]->update(_bvs[i], _lookup[i].second);

}


void
Population::plus_selection(const Population& offsprings)
{
  for (int
         i = 0,
         j = 0;
       i < int(_bvs.size()) && j < offsprings.size(); i++) {
    if (_compare_index_value(_lookup[i], offsprings._lookup[j]))
      continue;
    else {
      // _lookup[i].first is left unchanged
      _lookup[i].second = offsprings.get_best_value(j);
      _bvs[_lookup[i].first] = offsprings.get_best_bv(j);
      j++;
    }
  }
}


void
Population::plus_selection(Population& offsprings)
{
  for (int
         i = 0,
         j = 0;
       i < int(_bvs.size()) && j < offsprings.size(); i++) {
    if (_compare_index_value(_lookup[i], offsprings._lookup[j]))
      continue;
    else {
      // _lookup[i].first is left unchanged
      _lookup[i].second = offsprings.get_best_value(j);
      std::swap(_bvs[_lookup[i].first], offsprings.get_best_bv(j));
      j++;
    }
  }
}


void
Population::comma_selection(const Population& offsprings)
{
  assert(int(_bvs.size()) <= offsprings.size());

  for (size_t i = 0; i < _bvs.size(); i++) {
    _lookup[i].first = i;
    _lookup[i].second = offsprings.get_best_value(i);
    _bvs[i] = offsprings.get_best_bv(i);
  }

}


void
Population::comma_selection(Population& offsprings)
{
  assert(int(_bvs.size()) <= offsprings.size());

  for (size_t i = 0; i < _bvs.size(); i++) {
    _lookup[i].first = i;
    _lookup[i].second = offsprings.get_best_value(i);
    std::swap(_bvs[i], offsprings.get_best_bv(i));
  }

}
