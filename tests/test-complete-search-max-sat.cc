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

#include <random>

#include "hnco/algorithms/complete-search.hh"
#include "hnco/exception.hh"
#include "hnco/functions/collection/max-sat.hh"
#include "hnco/random.hh"

using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  const int num_runs = 100;

  Generator::set_seed();

  std::uniform_int_distribution<int> dist_bv_size(1, 15);
  std::uniform_int_distribution<int> dist_c(1, 100);

  for (int i = 0; i < num_runs; i++) {

    const int bv_size = dist_bv_size(Generator::engine);

    std::uniform_int_distribution<int> dist_k(1, bv_size);

    const int k = dist_k(Generator::engine);
    const int c = dist_c(Generator::engine);

    bit_vector_t solution(bv_size);
    bv_random(solution);
    MaxSat function;
    function.random(solution, k, c);

    CompleteSearch algorithm(bv_size);

    try {
      algorithm.maximize({&function});
    }
    catch (...) {
      return 1;
    }

    if (algorithm.get_solution().second != double(c))
      return 1;

  }

  return 0;
}
