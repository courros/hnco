/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#include <cassert>

#include "hnco/util.hh"         // hnco::have_same_size

#include "herding.hh"

using namespace hnco::algorithm::walsh_moment;

void
TriangularMomentHerding::init()
{
  _time = 0;
  _count.init();
  perm_identity(_permutation);
}

double
TriangularMomentHerding::error(const TriangularMoment& target)
{
  _error.scaled_difference(_time, target, _count);
  return _error.norm_2();
}

void
TriangularMomentHerding::sample(const TriangularMoment& target, bit_vector_t& bv)
{
  assert(hnco::have_same_size(bv, _permutation));

  const int bv_size = _permutation.size();

  if (_randomize_bit_order)
    perm_random(_permutation);

  _time++;
  _delta.scaled_difference(_time, target, _count);

  for (int k = 0; k < bv_size; k++) {
    int i = _randomize_bit_order ? _permutation[k] : k;
    double acc = 0;
    for (int l = 0; l < k; l++) {
      int j = _randomize_bit_order ? _permutation[l] : l;
      if (bv[j]) {
        if (j < i)
          acc -= _delta.second_moment[i][j];
        else
          acc -= _delta.second_moment[j][i];
      } else {
        if (j < i)
          acc += _delta.second_moment[i][j];
        else
          acc += _delta.second_moment[j][i];
      }
    }
    if (_delta.first_moment[i] + acc > 0)
      bv[i] = 0;
    else
      bv[i] = 1;
  }

  _count.add(bv);
}

void
FullMomentHerding::init()
{
  _time = 0;
  _count.init();
  perm_identity(_permutation);
}

double
FullMomentHerding::error(const FullMoment& target)
{
  _error.scaled_difference(_time, target, _count);
  return _error.norm_2();
}

void
FullMomentHerding::sample(const FullMoment& target, bit_vector_t& bv)
{
  assert(hnco::have_same_size(bv, _permutation));

  const int bv_size = _permutation.size();

  if (_randomize_bit_order)
    perm_random(_permutation);

  _time++;
  _delta.scaled_difference(_time, target, _count);

  for (int k = 0; k < bv_size; k++) {
    int i = _randomize_bit_order ? _permutation[k] : k;
    const auto& row = _delta.second_moment[i];
    double acc = 0;
    for (int l = 0; l < k; l++) {
      int j = _randomize_bit_order ? _permutation[l] : l;
      if (bv[j])
        acc -= row[j];
      else
        acc += row[j];
    }
    if (_delta.first_moment[i] + acc > 0)
      bv[i] = 0;
    else
      bv[i] = 1;
  }

  _count.add(bv);
}
