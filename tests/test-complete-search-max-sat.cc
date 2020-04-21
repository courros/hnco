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

#include <chrono>
#include <random>

#include "hnco/algorithms/complete-search.hh"
#include "hnco/exception.hh"
#include "hnco/functions/max-sat.hh"
#include "hnco/random.hh"

using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  const int num_runs = 100;

  Random::generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

  for (int i = 0; i < num_runs; i++) {

    std::uniform_int_distribution<int> dist_bv_size(1, 15);

    int bv_size = dist_bv_size(Random::generator);

    std::uniform_int_distribution<int> dist_k(1, bv_size);
    std::uniform_int_distribution<int> dist_c(1, 100);

    int k = dist_k(Random::generator);
    int c = dist_c(Random::generator);

    bit_vector_t solution(bv_size);
    bv_random(solution);

    MaxSat function;
    function.random(solution, k, c);

    CompleteSearch algorithm(bv_size);
    algorithm.set_function(&function);
    algorithm.init();
    try {
      algorithm.maximize();
    }
    catch (...) {
      return 1;
    }

    if (algorithm.get_solution().second != double(c))
      return 1;

  }

  return 0;
}
