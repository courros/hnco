/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025, 2026 Arnaud Berny

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

#include "modifier.hh"

#include "hnco/random.hh"

#include <iostream>

using namespace hnco::function;
using namespace hnco::function::modifier;

FunctionMapComposition::FunctionMapComposition(Function *function, hnco::map::Map *map)
  : Modifier(function)
  , _map(map)
{
  assert(map);

  if (map->get_output_size() != function->get_bv_size())
    throw std::runtime_error("FunctionMapComposition::FunctionMapComposition: _function and _map must be compatible");
  _output.resize(function->get_bv_size());
}

bool
FunctionMapComposition::has_known_maximum() const
{
  return
    _function->has_known_maximum() &&
    _map->is_surjective();
}

double
FunctionMapComposition::get_maximum() const
{
  if (has_known_maximum())
    return _function->get_maximum();
  else
    throw std::runtime_error("FunctionMapComposition::get_maximum: Unknown maximum");
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

ParsedModifier::ParsedModifier(Function *function, std::string expression)
  : Modifier(function)
{
  int result = _fparser.Parse(expression, "x");
  if (result != -1) {
    std::ostringstream stream;
    stream
      << "ParsedModifier::ParsedModifier: " << _fparser.ErrorMsg()
      << " at position: " << result
      << " in expression: " << expression;
    throw std::runtime_error(stream.str());
  }
}

double
ParsedModifier::evaluate(const bit_vector_t& x)
{
  _values[0] = _function->evaluate(x);
  return _fparser.Eval(_values);
}

double
PriorNoise::evaluate(const bit_vector_t& bv)
{
  _neighborhood->map(bv, _noisy_bv);
  return _function->evaluate(_noisy_bv);
}
