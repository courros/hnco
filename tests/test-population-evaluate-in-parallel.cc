/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

/** \file

    Check Population::evaluate_in_parallel=.

*/

#include <omp.h>                // omp_set_num_threads

#include <iostream>

#include "hnco/functions/theory.hh"
#include "hnco/map.hh"
#include "hnco/functions/modifiers/modifier.hh"
#include "hnco/algorithms/population.hh"

using namespace hnco::random;
using namespace hnco::algorithm;
using namespace hnco::function;
using namespace hnco::function::modifier;
using namespace hnco;

bool check()
{
  std::uniform_int_distribution<int> dist_population_size(1, 100);
  std::uniform_int_distribution<int> dist_n(1, 100);
  std::uniform_int_distribution<int> dist_num_threads(1, 100);

  for (int i = 0; i < 10; i++) {
    const int population_size   = dist_population_size  (Generator::engine);
    const int n                 = dist_n                (Generator::engine);
    const int num_threads       = dist_num_threads      (Generator::engine);

    omp_set_num_threads(num_threads);

    OneMax fn(n);

    Translation translation;
    translation.random(n);

    std::vector<Function *> fns(num_threads);
    for (auto& composition : fns) {
      composition = new FunctionMapComposition(&fn, &translation);
    }

    Population population_seq(population_size, n);
    assert(population_seq.size() == population_size);
    population_seq.random();

    Population population_par(population_seq);
    assert(population_seq.size() == population_par.size());

    population_seq.evaluate(fns[0]);
    population_par.evaluate_in_parallel(fns);

    for (int j = 0; j < population_size; j++) {
      assert(population_seq.get_bv(j) == population_par.get_bv(j));
      // Populations are not sorted so get_best_value returns the
      // value of get_bv with the same index.
      if (population_seq.get_best_value(j) != population_par.get_best_value(j))
        return false;
    }

    for (auto composition : fns) {
      delete composition;
    }

  }

  return true;
}

int main(int argc, char *argv[])
{
  Generator::set_seed();
  if (check())
    return 0;
  else
    return 1;
}
