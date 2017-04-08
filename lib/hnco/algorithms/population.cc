/* Copyright (C) 2016, 2017 Arnaud Berny

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
Population::eval(Function *function)
{
  assert(function);
  assert(_bvs.size() == _lookup.size());

  for (size_t i = 0; i < _bvs.size(); i++) {
    _lookup[i].first = i;
    _lookup[i].second = function->eval(_bvs[i]);
  }

}

void
Population::eval(const std::vector<function::Function *>& fns)
{
  assert(_bvs.size() == _lookup.size());
  assert(fns.size() > 1);

  for (size_t i = 0; i < _bvs.size(); i++)
    _lookup[i].first = i;

  size_t q = _bvs.size() / fns.size();
  size_t r = _bvs.size() % fns.size();
  assert(r < fns.size());

#pragma omp parallel for
  for (size_t i = 0; i < fns.size(); ++i) {
    // r threads call function (q + 1) times
    if (i < r)
      for (size_t j = 0; j < q + 1; j++) {
        size_t index = i * (q + 1) + j;
        _lookup[index].second = fns[i]->safe_eval(_bvs[index]);
      }
    // (fns.size() - r) threads call function q times
    else
      for (size_t j = 0; j < q; j++) {
        size_t index = r * (q + 1) + (i - r) * q + j;
        _lookup[index].second = fns[i]->safe_eval(_bvs[index]);
      }
  }

  for (size_t i = 0; i < _bvs.size(); i++)
    fns[0]->update(_bvs[i], _lookup[i].second);

}

inline
bool comp(const std::pair<size_t, double>& a,
          const std::pair<size_t, double>& b)
{
  return a.second > b.second;
}


void
Population::sort()
{
  std::sort(_lookup.begin(), _lookup.end(), comp);
}


void
Population::partial_sort(int selection_size)
{
  assert(selection_size > 0);
  std::partial_sort(_lookup.begin(), _lookup.begin() + selection_size, _lookup.end(), comp);
}


void
Population::plus_selection(const Population& offsprings)
{
  for (size_t
         i = 0,
         j = 0;
       i < _bvs.size() && j < offsprings.size(); i++) {
    if (comp(_lookup[i], offsprings._lookup[j]))
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
Population::comma_selection(const Population& offsprings)
{
  assert(_bvs.size() <= offsprings.size());

  for (size_t i = 0; i < _bvs.size(); i++) {
    _lookup[i].first = i;
    _lookup[i].second = offsprings.get_best_value(i);
    _bvs[i] = offsprings.get_best_bv(i);
  }

}


const bit_vector_t&
TournamentSelection::select()
{
  int winner = _choose_individual(Random::engine);
  for (int i = 0; i < _tournament_size; i++) {
    int challenger;
    do {
      challenger = _choose_individual(Random::engine);
    } while (challenger == winner);
    if (comp(_lookup[challenger], _lookup[winner]))
      winner = challenger;
  }
  return get_best_bv(winner);
}
