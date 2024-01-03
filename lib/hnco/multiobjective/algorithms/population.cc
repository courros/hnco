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

#include <omp.h>                // omp_get_thread_num

#include "hnco/util.hh"         // hnco::is_in_range, hnco::have_same_size

#include "population.hh"


using namespace hnco::multiobjective::algorithm;


void Population::random()
{
  for (auto& bv : bvs)
    bv_random(bv);
}

void Population::evaluate(Function *function)
{
  assert(have_same_size(bvs, values));
  assert(function);

  for (size_t i = 0; i < bvs.size(); i++)
    function->evaluate(bvs[i], values[i]);
}

void Population::evaluate_in_parallel(const std::vector<Function *>& functions)
{
  assert(have_same_size(bvs, values));
  assert(!functions.empty());

#pragma omp parallel for
  for (size_t i = 0; i < bvs.size(); i++) {
    const int k = omp_get_thread_num();
    assert(is_in_range(k, functions.size()));
    assert(functions[k]);
    functions[k]->evaluate(bvs[i], values[i]);
  }
}
