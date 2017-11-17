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

#include "hnco/random.hh"

#include "walsh-expansion-2.hh"


using namespace hnco::random;
using namespace hnco::function;


void
WalshExpansion2::random(int n, double stddev_lin, double stddev_quad)
{
  assert(n > 0);
  assert(stddev_lin > 0);
  assert(stddev_quad > 0);

  // Linear part
  _linear.resize(n);
  for (size_t i = 0; i < _linear.size(); i++)
    _linear[i] = stddev_lin * Random::normal();

  // Quadratic part
  _quadratic.resize(n);
  for (size_t i = 1; i < _quadratic.size(); i++) {
    _quadratic[i].resize(i);
    for (size_t j = 0; j < i; j++)
      _quadratic[i][j] = stddev_quad * Random::normal();
  }
}


double
WalshExpansion2::eval(const bit_vector_t& s)
{
  assert(s.size() == _linear.size());
  assert(s.size() == _quadratic.size());

  double result = 0;

  // Linear part
  for (size_t i = 0; i < _linear.size(); i++)
    if (s[i])
      result -= _linear[i];
    else
      result += _linear[i];

  // Quadratic part
  for (size_t i = 0; i < _quadratic.size(); i++) {
    auto& line = _quadratic[i];
    const bit_t b = s[i];
    for (size_t j = 0; j < i; j++)
      if (s[j] == b)
	result += line[j];
      else
	result -= line[j];
  }

  return result;
}
