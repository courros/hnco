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

#include "quadratic-function.hh"


using namespace std;
using namespace hnco::random;
using namespace hnco::function;


void
WalshExpansion2::random(int n, double stddev_lin, double stddev_quad)
{
  assert(n > 0);
  assert(stddev_lin > 0);
  assert(stddev_quad > 0);

  _linear.resize(n);
  _quadratic.resize(n, _linear);

  // Linear part
  for (size_t i = 0; i < _linear.size(); i++)
    _linear[i] = stddev_lin * Random::normal();

  // Quadratic part
  for (size_t i = 0; i < _quadratic.size(); i++)
    for (size_t j = i + 1; j < _quadratic.size(); j++)
      _quadratic[i][j] = stddev_quad * Random::normal();
}


double
WalshExpansion2::eval(const bit_vector_t& s)
{
  assert(s.size() == _linear.size());
  assert(s.size() == _quadratic.size());

  double r = 0;

  const size_t dimension = _linear.size();

  // Linear part
  for (size_t i = 0; i < dimension; i++)
    if (s[i])
      r += _linear[i];
    else
      r -= _linear[i];

  // Quadratic part
  for (size_t i = 0; i < dimension; i++) {
    auto line = _quadratic[i];
    bit_t b = s[i];
    for (size_t j = i + 1; j < dimension; j++)
      if (b == s[j])
	r += line[j];
      else
	r -= line[j];
  }

  return r;
}
