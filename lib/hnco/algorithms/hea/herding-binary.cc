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

#include <math.h>		// sqrt

#include "hnco/algorithms/util.hh"
#include "hnco/exception.hh"
#include "hnco/permutation.hh"

#include "herding-binary.hh"


using namespace hnco::algorithm::hea;
using namespace hnco::exception;
using namespace std;


void
BinaryHerding::init()
{
  _time = 0;
  _count.init();
}


double
BinaryHerding::error(const BinaryMoment& target)
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
BinaryHerding::sample(const BinaryMoment& target, bit_vector_t& x)
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
    ostringstream stream;
    stream << _dynamics;
    throw Error("BinaryHerding::sample: Unknown _dynamics: " + stream.str());
  }
}


void
BinaryHerding::compute_delta(const BinaryMoment& target)
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
BinaryHerding::sample_minimize_norm(const BinaryMoment& target, bit_vector_t& x)
{
  if (_randomize_bit_order)
    perm_random(_permutation);

  for (size_t k = 0; k < _delta.size(); k++) {
    size_t i = _randomize_bit_order ? _permutation[k] : k;
    double lambda = 1 - 2 * _delta._moment[i][i];
    double off_diagonal = 0;
    for (size_t l = 0; l < k; l++) {
      size_t j = _randomize_bit_order ? _permutation[l] : l;
      if (x[j])
	off_diagonal += 1 - 2 * _delta._moment[i][j];
    }
    lambda += 2 * _weight * off_diagonal;
    if (lambda < 0) {
      x[i] = 1;
      _count._moment[i][i]++;
      for (size_t l = 0; l < k; l++) {
	size_t j = _randomize_bit_order ? _permutation[l] : l;
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
BinaryHerding::sample_maximize_inner_product(const BinaryMoment& target, bit_vector_t& x)
{
  if (_randomize_bit_order)
    perm_random(_permutation);

  for (size_t k = 0; k < _delta.size(); k++) {
    size_t i = _randomize_bit_order ? _permutation[k] : k;
    double lambda = _delta._moment[i][i];
    double off_diagonal = 0;
    for (size_t l = 0; l < k; l++) {
      size_t j = _randomize_bit_order ? _permutation[l] : l;
      if (x[j])
	off_diagonal += _delta._moment[i][j];
    }
    lambda += 2 * _weight * off_diagonal;
    if (lambda > 0) {
      x[i] = 1;
      _count._moment[i][i]++;
      for (size_t l = 0; l < k; l++) {
	size_t j = _randomize_bit_order ? _permutation[l] : l;
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
