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

#include <math.h>		// sqrt

#include "hnco/exception.hh"
#include "hnco/permutation.hh"
#include "hnco/util.hh"

#include "bit-herding.hh"


using namespace hnco::algorithm::hea;
using namespace hnco::exception;


void
BitHerding::init()
{
  _time = 0;
  _count.init();
}


double
BitHerding::error(const BitMoment& target)
{
  assert(target.size() == _count.size());

  double result = 0;
  for (size_t i = 0; i < _count.size(); i++) {
    result += square(_time * target._moment[i][i] - _count._moment[i][i]);
    double tmp = 0;
    for (size_t j = 0; j < i; j++)
      tmp += square(_time * target._moment[i][j] - _count._moment[i][j]);
    result += _weight * tmp;
  }
  return sqrt(result) / (_time * _count.diameter()) ;
}


void
BitHerding::sample(const BitMoment& target, bit_vector_t& x)
{
  assert(target.size() == _count.size());

  compute_delta(target);

  switch (_dynamics) {
  case DYNAMICS_MINIMIZE_NORM:
    sample_minimize_norm(target, x);
    break;
  case DYNAMICS_MAXIMIZE_INNER_PRODUCT:
    sample_maximize_inner_product(target, x);
    break;
  default:
    std::ostringstream stream;
    stream << _dynamics;
    throw Error("BitHerding::sample: Unknown _dynamics: " + stream.str());
  }
}


void
BitHerding::compute_delta(const BitMoment& target)
{
  _time++;

  for (size_t i = 0; i < _count.size(); i++) {
    _delta._moment[i][i] = _time * target._moment[i][i] - _count._moment[i][i];
    for (size_t j = 0; j < i; j++) {
      _delta._moment[i][j] = _time * target._moment[i][j] - _count._moment[i][j];
      _delta._moment[j][i] = _delta._moment[i][j];
    }
  }

  assert(matrix_is_symmetric(_delta._moment));
}


void
BitHerding::sample_minimize_norm(const BitMoment& target, bit_vector_t& x)
{
  if (_randomize_bit_order)
    perm_random(_permutation);

  for (int k = 0; k < int(_delta.size()); k++) {
    int i = _randomize_bit_order ? _permutation[k] : k;
    double lambda = 1 - 2 * _delta._moment[i][i];
    double off_diagonal = 0;
    for (int l = 0; l < k; l++) {
      int j = _randomize_bit_order ? _permutation[l] : l;
      if (x[j])
	off_diagonal += 1 - 2 * _delta._moment[i][j];
    }
    lambda += 2 * _weight * off_diagonal;
    if (lambda < 0) {
      x[i] = 1;
      _count._moment[i][i]++;
      for (int l = 0; l < k; l++) {
	int j = _randomize_bit_order ? _permutation[l] : l;
        assert(j != i);
	if (x[j]) {
          _count._moment[i][j]++;
          _count._moment[j][i] = _count._moment[i][j];
	}
      }
    }
    else
      x[i] = 0;
  }

  assert(matrix_is_symmetric(_count._moment));
  assert(matrix_has_dominant_diagonal(_count._moment));
}


void
BitHerding::sample_maximize_inner_product(const BitMoment& target, bit_vector_t& x)
{
  if (_randomize_bit_order)
    perm_random(_permutation);

  for (int k = 0; k < int(_delta.size()); k++) {
    int i = _randomize_bit_order ? _permutation[k] : k;
    double lambda = _delta._moment[i][i];
    double off_diagonal = 0;
    for (int l = 0; l < k; l++) {
      int j = _randomize_bit_order ? _permutation[l] : l;
      if (x[j])
	off_diagonal += _delta._moment[i][j];
    }
    lambda += 2 * _weight * off_diagonal;
    if (lambda > 0) {
      x[i] = 1;
      _count._moment[i][i]++;
      for (int l = 0; l < k; l++) {
	int j = _randomize_bit_order ? _permutation[l] : l;
        assert(j != i);
	if (x[j]) {
          _count._moment[i][j]++;
          _count._moment[j][i] = _count._moment[i][j];
	}
      }
    }
    else
      x[i] = 0;
  }

  assert(matrix_is_symmetric(_count._moment));
  assert(matrix_has_dominant_diagonal(_count._moment));
}
