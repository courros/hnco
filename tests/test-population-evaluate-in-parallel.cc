/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#include "hnco/functions/collection/theory.hh"
#include "hnco/maps/map.hh"
#include "hnco/functions/modifiers/modifier.hh"
#include "hnco/algorithms/population.hh"

using namespace hnco::algorithm;
using namespace hnco::function::modifier;
using namespace hnco::function;
using namespace hnco::map;
using namespace hnco::random;
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

    std::vector<Function *> oms(num_threads);
    std::vector<Map *> trs(num_threads);
    std::vector<Function *> fns(num_threads);

    // Simulate how functions are created in hnco.cc
    for (int k = 0; k < num_threads; k++) {
      Generator::reset();
      oms[k] = new OneMax(n);
      auto tr = new Translation();
      tr->random(n);
      trs[k] = tr;
      fns[k] = new FunctionMapComposition(oms[k], trs[k]);
    }

    Population population_seq(population_size, n);
    assert(population_seq.get_size() == population_size);
    assert(population_seq.get_bv_size() == n);
    population_seq.random();

    Population population_par(population_seq);
    assert(population_par.get_size() == population_size);
    assert(population_par.get_bv_size() == n);

    population_seq.evaluate(fns[0]);
    population_par.evaluate_in_parallel(fns);

    for (int j = 0; j < population_size; j++) {
      assert(population_seq.bvs[j] == population_par.bvs[j]);
      if (population_seq.values[j] != population_par.values[j])
        return false;
    }

    for (int k = 0; k < num_threads; k++) {
      delete fns[k];
      delete trs[k];
      delete oms[k];
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
