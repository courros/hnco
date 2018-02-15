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

#include "hnco/random.hh"

#include "linear-function.hh"


using namespace hnco::function;
using namespace hnco::random;


void
LinearFunction::random(int n)
{
  assert(n > 0);

  _weights.resize(n);
  for (size_t i = 0; i < _weights.size(); i++)
    _weights[i] = Random::uniform();
}


double
LinearFunction::eval(const bit_vector_t& x)
{
  assert(x.size() == _weights.size());

  double result = 0;
  for (size_t i = 0; i < x.size(); i++)
    if (x[i])
      result += _weights[i];
  return result;
}


double
LinearFunction::get_maximum()
{
  double result = 0;
  for (size_t i = 0; i < _weights.size(); i++)
    if (_weights[i] > 0)
      result += _weights[i];
  return result;
}
