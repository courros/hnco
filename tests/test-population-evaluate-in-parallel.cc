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

#include <iostream>

#include "hnco/algorithms/population.hh"
#include "hnco/functions/theory.hh"


using namespace hnco::random;
using namespace hnco::algorithm;
using namespace hnco;

bool check()
{
  std::uniform_int_distribution<int> dist_population_size(2, 10);
  std::uniform_int_distribution<int> dist_n(2, 10);
  std::uniform_int_distribution<int> dist_num_threads(1, 5);

  for (int i = 0; i < 10; i++) {
    const int population_size   = dist_population_size  (Generator::engine);
    const int n                 = dist_n                (Generator::engine);
    const int num_threads       = dist_num_threads      (Generator::engine);

    Population population_seq(population_size, n);
    population_seq.random();

    Population population_par(population_seq);

    function::OneMax fn(n);
    std::vector<function::Function *> fns(num_threads, &fn);

    population_seq.evaluate(&fn);
    population_par.evaluate_in_parallel(fns);
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
