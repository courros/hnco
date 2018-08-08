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

#include <assert.h>		// assert

#include "hnco/algorithms/matrix.hh"
#include "hnco/util.hh"

#include "moment-binary.hh"

using namespace hnco::algorithm::hea;
using namespace hnco::algorithm;


void
BinaryMoment::uniform()
{
  for (size_t i = 0; i < _moment.size(); i++) {
    _moment[i][i] = 0.5;
    for (size_t j = 0; j < i; j++) {
      _moment[i][j] = 0.25;
      _moment[j][i] = 0.25;
    }
  }
}


void
BinaryMoment::init()
{
  for (size_t i = 0; i < _moment.size(); i++) {
    _moment[i][i] = 0;
    for (size_t j = 0; j < i; j++) {
      _moment[i][j] = 0;
      _moment[j][i] = 0;
    }
  }
}


void
BinaryMoment::add(const bit_vector_t& x)
{
  assert(x.size() == _moment.size());

  for (size_t i = 0; i < _moment.size(); i++) {
    if (x[i] == 0)
      continue;
    _moment[i][i]++;
    for (size_t j = 0; j < i; j++) {
      if (x[j]) {
	_moment[i][j]++;
        _moment[j][i] = _moment[i][j];
      }
    }
  }

  assert(matrix_is_symmetric(_moment));
  assert(matrix_has_dominant_diagonal(_moment));
}


void
BinaryMoment::average(int count)
{
  for (size_t i = 0; i < _moment.size(); i++) {
    _moment[i][i] /= count;
    for (size_t j = 0; j < i; j++) {
      _moment[i][j] /= count;
      _moment[j][i] = _moment[i][j];
    }
  }

  assert(matrix_is_symmetric(_moment));
  assert(matrix_has_range(_moment, 0.0, 1.0));
  assert(matrix_has_dominant_diagonal(_moment));
}


void
BinaryMoment::update(const BinaryMoment& p, double rate)
{
  for (size_t i = 0; i < _moment.size(); i++) {
    _moment[i][i] += rate * (p._moment[i][i] - _moment[i][i]);
    for (size_t j = 0; j < i; j++) {
      _moment[i][j] += rate * (p._moment[i][j] - _moment[i][j]);
      _moment[j][i] = _moment[i][j];
    }
  }

  assert(matrix_is_symmetric(_moment));
  assert(matrix_has_range(_moment, 0.0, 1.0));
  assert(matrix_has_dominant_diagonal(_moment));
}


void
BinaryMoment::bound(double margin)
{
  const double high = 1 - margin;
  const double low = margin;

  assert(low > 0);
  assert(high < 1);
  assert(low < high);

  for (size_t i = 0; i < _moment.size(); i++) {
    double tmp = _moment[i][i];
    tmp = std::min(high, tmp);
    tmp = std::max(low, tmp);
    _moment[i][i] = tmp;
  }

  for (size_t i = 0; i < _moment.size(); i++) {
    for (size_t j = 0; j < i; j++) {
      double tmp = _moment[i][j];
      tmp = std::min(high, tmp);
      tmp = std::max(low, tmp);
      _moment[i][j] = tmp;
      _moment[j][i] = tmp;
    }
  }

  assert(matrix_is_symmetric(_moment));
  assert(matrix_has_range(_moment, low, high));
}


double
BinaryMoment::distance(const BinaryMoment& p) const
{
  assert(p._moment.size() == _moment.size());

  double result = 0;
  for (size_t i = 0; i < _moment.size(); i++) {
    result += square(_moment[i][i] - p._moment[i][i]);
    double tmp = 0;
    for (size_t j = 0; j < i; j++)
      tmp += square(_moment[i][j] - p._moment[i][j]);
    result += _weight * tmp;
  }
  return sqrt(result) / diameter();
}


double
BinaryMoment::norm_2() const
{
  double result = 0;
  for (size_t i = 0; i < _moment.size(); i++) {
    result += square(_moment[i][i]);
    double tmp = 0;
    for (size_t j = 0; j < i; j++)
      tmp += square(_moment[i][j]);
    result += _weight * tmp;
  }
  return sqrt(result) / diameter();
}
