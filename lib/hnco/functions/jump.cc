/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#include "hnco/exception.hh"

#include "jump.hh"


using namespace hnco::function;
using namespace hnco::exception;


Jump::Jump(int bv_size, int gap):
  _bv_size(bv_size),
  _gap(gap)
{
  assert(bv_size > 0);

  if (_gap < 1)
    throw Error("Jump::Jump: _gap must be >= 1");
}


double
Jump::eval(const bit_vector_t& x)
{
  assert(int(x.size()) == _bv_size);

  int weight = bv_hamming_weight(x);

  if (weight == int(x.size()))
    return weight;

  assert(weight < int(x.size()));

  if (weight > int(x.size()) - _gap)
    return 0;

  return weight;
}


DeceptiveJump::DeceptiveJump(int bv_size, int gap):
  _bv_size(bv_size),
  _gap(gap)
{
  assert(bv_size > 0);

  if (_gap < 1)
    throw Error("Jump::Jump: _gap must be >= 1");
}


double
DeceptiveJump::eval(const bit_vector_t& x)
{
  assert(int(x.size()) == _bv_size);

  int weight = bv_hamming_weight(x);

  if (weight < int(x.size()) &&
      weight > int(x.size()) - _gap)
    return x.size() - weight;
  else
    return _gap + weight;
}
