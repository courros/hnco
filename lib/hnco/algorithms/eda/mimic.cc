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

#include <algorithm>            // std::fill

#include "hnco/random.hh"

#include "mimic.hh"


using namespace hnco::algorithm::eda;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


void
Mimic::init()
{
  random_solution();
  perm_random(_permutation);
  std::fill(_parameters.begin(), _parameters.end(), std::make_pair(0.5, 0.5));
}

void
Mimic::sample(bit_vector_t& bv)
{
  assert(bv.size() == _permutation.size());
  assert(bv.size() == _parameters.size());
  assert(perm_is_valid(_permutation));

  // First component
  bv[_permutation[0]] = (Random::uniform() < _parameters[0].first) ? 1 : 0;

  for (std::size_t i = 1; i < bv.size(); i++)
    if (bv[i - 1])
      bv[_permutation[i]] = (Random::uniform() < _parameters[i].first) ? 1 : 0;
    else
      bv[_permutation[i]] = (Random::uniform() < _parameters[i].second) ? 1 : 0;
}

void
Mimic::iterate()
{
  for (size_t i = 0; i < _population.size(); i++)
    sample(_population.get_bv(i));

  if (_functions.size() > 1)
    _population.eval(_functions);
  else
    _population.eval(_function);

  _population.sort();

  update_solution(_population.get_best_bv(),
                  _population.get_best_value());

  // update model
}
