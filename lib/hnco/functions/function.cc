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

#include "function.hh"

#include "hnco/iterator.hh"

#include <algorithm>            // std::copy, std::copy_backward

using namespace hnco::function;
using namespace hnco;

void
Function::display(std::ostream& stream) const
{
  stream << "Function: " << get_bv_size() << " bits -> double" << std::endl;
}

bool
hnco::function::bv_is_locally_maximal(const bit_vector_t& bv, Function& fn, hnco::neighborhood::NeighborhoodIterator& it)
{
  double value = fn.evaluate(bv);
  it.set_origin(bv);
  it.init();
  while (it.has_next()) {
    if (fn.evaluate(it.next()) > value)
      return false;
  }
  return true;
}

bool
hnco::function::bv_is_globally_maximal(const bit_vector_t& bv, Function& fn)
{
  assert(fn.has_known_maximum());
  return fn.evaluate(bv) == fn.get_maximum();
}
