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

#include <algorithm>            // std::equal_range
#include <iterator>             // std::distance

#include "hnco/util.hh"         // hnco::is_in_range

#include "population.hh"

using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;

void
Population::random()
{
  for (auto& bv : bvs)
    bv_random(bv);
}

void
Population::evaluate(Function *function)
{
  assert(have_same_size(bvs, values));
  assert(function);

  for (size_t i = 0; i < bvs.size(); i++)
    values[i] = function->evaluate(bvs[i]);
}

void
Population::evaluate_in_parallel(const std::vector<Function *>& fns)
{
  assert(have_same_size(bvs, values));
  assert(!fns.empty());

#pragma omp parallel for
  for (size_t i = 0; i < bvs.size(); i++) {
    int k = omp_get_thread_num();
    assert(is_in_range(k, fns.size()));
    assert(fns[k]);

    values[i] = fns[k]->evaluate_safely(bvs[i]);
  }

  for (size_t i = 0; i < bvs.size(); i++)
    fns[0]->update(bvs[i], values[i]);
}

std::pair<int, int>
Population::get_equivalent_bvs(int index) const
{
  assert(is_in_range(index, permutation.size()));

  auto compare = [this](int i, int j){ return this->values[i] > this->values[j]; };
  auto range = std::equal_range(permutation.begin(), permutation.end(), permutation[index], compare);

  return std::make_pair(std::distance(permutation.begin(), range.first),
                        std::distance(permutation.begin(), range.second));
}
