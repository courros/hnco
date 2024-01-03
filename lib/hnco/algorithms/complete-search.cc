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

#include "hnco/exception.hh"
#include "hnco/iterator.hh"

#include "complete-search.hh"

using namespace hnco::algorithm;
using namespace hnco;


void
CompleteSearch::maximize(const std::vector<function::Function *>& functions)
{
  set_functions(functions);

  HypercubeIterator iterator(get_bv_size());

  if (!iterator.has_next())
    throw std::runtime_error("CompleteSearch::maximize: empty hypercube");

  set_solution(iterator.next());

  while (iterator.has_next())
    update_solution(iterator.next());
}
