/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#include <algorithm>            // std::max, std::fill

#include "hnco/algorithms/matrix.hh" // hnco::algorithm::matrix_is_symmetric
#include "hnco/random.hh"
#include "hnco/util.hh"         // hnco::logistic, hnco::is_in_range

#include "model.hh"


using namespace hnco;
using namespace hnco::random;
using namespace hnco::algorithm::bm_pbil;

void
GibbsSampler::init()
{
  bv_random(_state);
}

void
GibbsSampler::update(int i)
{
  assert(is_in_range(i, 0, _state.size()));

  double delta = _model_parameters.first_moment[i];
  const std::vector<double>& row = _model_parameters.second_moment[i];
  for (size_t j = 0; j < _state.size(); j++) {
    if (int(j) == i)
      continue;
    if (_state[j])
      delta -= row[j];
    else
      delta += row[j];
  }
  delta *= -2;

  if (Generator::uniform() < logistic(delta))
    _state[i] = 1;
  else
    _state[i] = 0;
}

void
GibbsSampler::update_sync()
{
  for (size_t i = 0; i < _pv.size(); i++) {
    double delta = _model_parameters.first_moment[i];
    const std::vector<double>& row = _model_parameters.second_moment[i];
    for (size_t j = 0; j < _state.size(); j++) {
      if (j == i)
        continue;
      if (_state[j])
        delta -= row[j];
      else
        delta += row[j];
    }
    delta *= -2;

    _pv[i] = logistic(delta);
  }
  pv_sample(_state, _pv);
}
