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

#include "hnco/random.hh"

#include "modifier.hh"


using namespace hnco::exception;
using namespace hnco::function;


double
Negation::eval(const bit_vector_t& x)
{
  return -_function->eval(x);
}


double
Negation::delta(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  return -_function->delta(x, value, flipped_bits);
}


double
FunctionMapComposition::eval(const bit_vector_t& x)
{
  _map->map(x, _bv);
  return _function->eval(_bv);
}


double
AdditiveGaussianNoise::eval(const bit_vector_t& x)
{
  return _function->eval(x) + _dist(random::Random::engine);
}
