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

#include <assert.h>		// assert

#include "hnco/algorithms/matrix.hh"
#include "hnco/util.hh"

#include "spin-moment.hh"

using namespace hnco::algorithm::hea;


void
SpinMoment::init()
{
  for (size_t i = 0; i < _first.size(); i++) {
    _first[i] = 0;

    std::vector<double>& line = _second[i];
    for (size_t j = 0; j < i; j++)
      line[j] = 0;
  }

  assert(matrix_is_strictly_lower_triangular(_second));
}


void
SpinMoment::add(const bit_vector_t& x)
{
  assert(x.size() == _first.size());

  for (size_t i = 0; i < x.size(); i++) {
    std::vector<double>& line = _second[i];
    if (x[i]) {
      _first[i]--;
      for (size_t j = 0; j < i; j++)
        if (x[j])
          line[j]++;
        else
          line[j]--;
    } else {
      _first[i]++;
      for (size_t j = 0; j < i; j++)
        if (x[j])
          line[j]--;
        else
          line[j]++;
    }
  }

  assert(matrix_is_strictly_lower_triangular(_second));
}


void
SpinMoment::average(int count)
{
  for (size_t i = 0; i < _first.size(); i++) {
    _first[i] /= count;
    assert(_first[i] >= -1 && _first[i] <= 1);

    std::vector<double>& line = _second[i];
    for (size_t j = 0; j < i; j++) {
      line[j] /= count;
      assert(line[j] >= -1 && line[j] <= 1);
    }
  }

  assert(matrix_is_strictly_lower_triangular(_second));
}


void
SpinMoment::update(const SpinMoment& p, double rate)
{
  for (size_t i = 0; i < _first.size(); i++) {
    _first[i] += rate * (p._first[i] - _first[i]);
    assert(_first[i] >= -1 && _first[i] <= 1);

    std::vector<double>& line = _second[i];
    const std::vector<double>& line2 = p._second[i];
    for (size_t j = 0; j < i; j++) {
      line[j] += rate * (line2[j] - line[j]);
      assert(line[j] >= -1 && line[j] <= 1);
    }
  }

  assert(matrix_is_strictly_lower_triangular(_second));
}


void
SpinMoment::bound(double margin)
{
  const double high = 1 - margin;
  const double low = margin - 1;

  for (size_t i = 0; i < _second.size(); i++) {
    double tmp = _first[i];
    tmp = std::min(high, tmp);
    tmp = std::max(low, tmp);
    _first[i] = tmp;

    std::vector<double>& line = _second[i];
    for (size_t j = 0; j < i; j++) {
      double tmp = line[j];
      tmp = std::min(high, tmp);
      tmp = std::max(low, tmp);
      line[j] = tmp;
    }
  }

  assert(matrix_is_strictly_lower_triangular(_second));
}


double
SpinMoment::distance(const SpinMoment& moment) const
{
  assert(moment._first.size() == _first.size());
  assert(moment._second.size() == _second.size());

  double result = 0;
  for (size_t i = 0; i < _first.size(); i++) {
    result += square(_first[i] - moment._first[i]);

    double tmp = 0;
    const std::vector<double>& line = _second[i];
    const std::vector<double>& line2 = moment._second[i];
    for (size_t j = 0; j < i; j++)
      tmp += square(line[j] - line2[j]);

    result += _weight * tmp;
  }

  return sqrt(result) / diameter();
}


double
SpinMoment::norm_2() const
{
  double result = 0;
  for (size_t i = 0; i < _first.size(); i++) {
    result += square(_first[i]);

    double tmp = 0;
    const std::vector<double>& line = _second[i];
    for (size_t j = 0; j < i; j++)
      tmp += square(line[j]);

    result += _weight * tmp;
  }

  return sqrt(result) / diameter();
}


void
SpinMoment::display(std::ostream& stream)
{
  for (size_t i = 0; i < _first.size(); i++) {
    for (size_t j = 0; j < i; j++)
      stream << _second[i][j] << " ";
    stream << _first [i] << " ";
    for (size_t j = i + 1; j < _first.size(); j++)
      stream << _second[j][i] << " ";
    stream << std::endl;
  }
  stream << std::endl;
  stream << std::endl;
}
