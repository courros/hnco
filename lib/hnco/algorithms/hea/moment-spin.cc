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

#include <assert.h>		// assert

#include "hnco/algorithms/matrix.hh"
#include "hnco/algorithms/util.hh"

#include "moment-spin.hh"

using namespace std;
using namespace hnco::algorithm::hea;


void
SpinMoment::uniform()
{
  for (size_t i = 0; i < _second.size(); i++) {
    _first[i] = 0;
    _second[i][i] = 1;
    for (size_t j = 0; j < i; j++) {
      _second[i][j] = 0;
      _second[j][i] = 0;
    }
  }

  assert(matrix_is_symmetric(_second));
  assert(matrix_has_range(_second, -1.0, 1.0));
}


void
SpinMoment::init()
{
  for (size_t i = 0; i < _first.size(); i++)
    _first[i] = 0;

  for (size_t i = 0; i < _second.size(); i++)
    for (size_t j = 0; j < i; j++) {
      _second[i][j] = 0;
      _second[j][i] = 0;
    }

  assert(matrix_is_symmetric(_second));
  assert(matrix_has_range(_second, -1.0, 1.0));
}


void
SpinMoment::add(const bit_vector_t& x)
{
  assert(x.size() == _second.size());

  for (size_t i = 0; i < _first.size(); i++)
    if (x[i])
      _first[i]++;
    else
      _first[i]--;

  for (size_t i = 0; i < _second.size(); i++) {
    for (size_t j = 0; j < i; j++) {
      if (x[j] == x[i])
	_second[i][j]++;
      else
        _second[i][j]--;
      _second[j][i] = _second[i][j];
    }
  }

  assert(matrix_is_symmetric(_second));
  assert(matrix_has_range(_second, -1.0, 1.0));
}


void
SpinMoment::average(int count)
{
  for (size_t i = 0; i < _first.size(); i++) {
    _first[i] /= count;
    assert(_first[i] >= -1 && _first[i] <= 1);
  }

  for (size_t i = 0; i < _second.size(); i++) {
    for (size_t j = 0; j < i; j++) {
      _second[i][j] /= count;
      _second[j][i] = _second[i][j];
    }
  }

  assert(matrix_is_symmetric(_second));
  assert(matrix_has_range(_second, -1.0, 1.0));
}


void
SpinMoment::update(const SpinMoment& p, double rate)
{
  for (size_t i = 0; i < _first.size(); i++) {
    _first[i] += rate * (p._first[i] - _first[i]);
    assert(_first[i] >= -1 && _first[i] <= 1);
  }

  for (size_t i = 0; i < _second.size(); i++) {
    for (size_t j = 0; j < i; j++) {
      _second[i][j] += rate * (p._second[i][j] - _second[i][j]);
      _second[j][i] = _second[i][j];
    }
  }

  assert(matrix_is_symmetric(_second));
  assert(matrix_has_range(_second, -1.0, 1.0));
}


void
SpinMoment::bound(double margin)
{
  const double high = 1 - margin;
  const double low = margin - 1;

  for (size_t i = 0; i < _first.size(); i++) {
    double tmp = _first[i];
    tmp = std::min(high, tmp);
    tmp = std::max(low, tmp);
    _first[i] = tmp;
  }

  for (size_t i = 0; i < _second.size(); i++) {
    for (size_t j = 0; j < i; j++) {
      double tmp = _second[i][j];
      tmp = std::min(high, tmp);
      tmp = std::max(low, tmp);
      _second[i][j] = tmp;
      _second[j][i] = tmp;
    }
  }

  assert(matrix_is_symmetric(_second));
  assert(matrix_has_range(_second, low, high));
}


double
SpinMoment::distance(const SpinMoment& moment) const
{
  assert(_first.size() == moment._first.size());
  assert(_second.size() == moment._second.size());

  double result = 0;
  for (size_t i = 0; i < _first.size(); i++) {
    result += square(_first[i] - moment._first[i]);
    double tmp = 0;
    for (size_t j = 0; j < i; j++)
      tmp += square(_second[i][j] - moment._second[i][j]);
    result += _weight * tmp;
  }
  return sqrt(result) / diameter() ;
}


double
SpinMoment::norm_2() const
{
  double result = 0;
  for (size_t i = 0; i < _first.size(); i++) {
    result += square(_first[i]);
    double tmp = 0;
    for (size_t j = 0; j < i; j++)
      tmp += square(_second[i][j]);
    result += _weight * tmp;
  }
  return sqrt(result) / diameter() ;
}
