/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#include <iostream>

#include "hnco/random.hh"

#include "modifier.hh"

using namespace hnco::function;
using namespace hnco::function::modifier;

double
OppositeFunction::evaluate(const bit_vector_t& bv)
{
  return -_function->evaluate(bv);
}

double
OppositeFunction::evaluate_incrementally(const bit_vector_t& bv, double value, const hnco::sparse_bit_vector_t& flipped_bits)
{
  return -_function->evaluate_incrementally(bv, -value, flipped_bits);
}

double
FunctionMapComposition::evaluate(const bit_vector_t& bv)
{
  _map->map(bv, _output);
  return _function->evaluate(_output);
}

void
FunctionMapComposition::describe(const bit_vector_t& bv, std::ostream& stream)
{
  assert(int(bv.size()) == get_bv_size());
  _map->map(bv, _output);
  return _function->describe(_output, stream);
}

double
AdditiveGaussianNoise::evaluate(const bit_vector_t& bv)
{
  return _function->evaluate(bv) + _dist(random::Generator::engine);
}
