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

#include "algorithm.hh"


using namespace hnco::algorithm;
using namespace hnco::random;


void
Algorithm::random_solution()
{
  assert(_function);

  bv_random(_solution.first);
  _solution.second = _function->evaluate(_solution.first);
}

void
Algorithm::set_solution(const bit_vector_t& bv, double value)
{
  _solution.first = bv;
  _solution.second = value;
}

void
Algorithm::set_solution(const bit_vector_t& bv)
{
  assert(_function);

  set_solution(bv, _function->evaluate(bv));
}

void
Algorithm::update_solution(const bit_vector_t& bv, double value)
{
  if (value > _solution.second) {
    _solution.first = bv;
    _solution.second = value;
  }
}

void
Algorithm::update_solution(const solution_t& s)
{
  if (s.second > _solution.second)
    _solution = s;
}

void
Algorithm::update_solution(const bit_vector_t& bv)
{
  assert(_function);

  update_solution(bv, _function->evaluate(bv));
}
