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

#include "hnco/random.hh"

#include "function-modifier.hh"


using namespace hnco::exception;
using namespace hnco::function;


double
Negation::eval(const bit_vector_t& x)
{
  return -_function->eval(x);
}


double
Negation::incremental_eval(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  return -_function->incremental_eval(x, -value, flipped_bits);
}


double
FunctionMapComposition::eval(const bit_vector_t& x)
{
  _map->map(x, _bv);
  return _function->eval(_bv);
}

void
FunctionMapComposition::describe(const bit_vector_t& x, std::ostream& stream)
{
  assert(int(x.size()) == get_bv_size());

  _map->map(x, _bv);
  return _function->describe(_bv, stream);
}


double
AdditiveGaussianNoise::eval(const bit_vector_t& x)
{
  return _function->eval(x) + _dist(random::Random::generator);
}
