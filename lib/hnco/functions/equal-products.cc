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

#include <assert.h>
#include <math.h>		// fabs

#include "hnco/random.hh"
#include "hnco/functions/equal-products.hh"


using namespace hnco::random;
using namespace hnco::function;


void
EqualProducts::random(int n, double upper_bound)
{
  assert(n > 0);
  assert(upper_bound > 0);

  _numbers.resize(n);
  for (size_t i = 0; i < _numbers.size(); i++)
    _numbers[i] = upper_bound * Random::uniform();
}


double
EqualProducts::eval(const bit_vector_t& s)
{
  double p0 = 1;
  double p1 = 1;
  for (size_t i = 0; i < _numbers.size(); i++)
    if (s[i])
      p1 *= _numbers[i];
    else
      p0 *= _numbers[i];
  return -fabs(p1 - p0);
}
