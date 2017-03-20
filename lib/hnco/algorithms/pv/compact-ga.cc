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

#include <utility>

#include "compact-ga.hh"

using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco;

void
CompactGa::init()
{
  random_solution();
  pv_uniform(_pv);
}

void
CompactGa::iterate()
{
  pv_sample(_pv, _candidates[0]);
  pv_sample(_pv, _candidates[1]);

  double v0 = _function->eval(_candidates[0]);
  double v1 = _function->eval(_candidates[1]);

  int best = 0;
  int worst = 1;

  if (v0 < v1) {
    std::swap(best, worst);
    update_solution(_candidates[1], v1);
  } else
    update_solution(_candidates[0], v0);

  assert(_function->eval(_candidates[best]) >= _function->eval(_candidates[worst]));

  // Update probability vector
  for (size_t i = 0; i < _pv.size(); i++)
    if (_candidates[best][i] != _candidates[worst][i]) {
      if (_candidates[best][i])
        _pv[i] += _rate;
      else
        _pv[i] -= _rate;
    }

  pv_bound(_pv, _lower_bound, _upper_bound);
}
