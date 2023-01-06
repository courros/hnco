/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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
using namespace hnco::function::modifier;


double
Negation::evaluate(const bit_vector_t& x)
{
  return -_function->evaluate(x);
}


double
Negation::evaluate_incrementally(const bit_vector_t& x, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  return -_function->evaluate_incrementally(x, -value, flipped_bits);
}


double
FunctionMapComposition::evaluate(const bit_vector_t& x)
{
  _map->map(x, _bv);
  return _function->evaluate(_bv);
}

void
FunctionMapComposition::describe(const bit_vector_t& x, std::ostream& stream)
{
  assert(int(x.size()) == get_bv_size());

  _map->map(x, _bv);
  return _function->describe(_bv, stream);
}


double
AdditiveGaussianNoise::evaluate(const bit_vector_t& x)
{
  return _function->evaluate(x) + _dist(random::Generator::engine);
}
