/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#include "hnco/exception.hh"
#include "hnco/functions/max-sat.hh"
#include "hnco/random.hh"

using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;
using namespace std;


int main(int argc, char *argv[])
{
  Random::engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

  uniform_int_distribution<int> dist_n(10, 1000);
  uniform_int_distribution<int> dist_k(2, 10);
  uniform_int_distribution<int> dist_c(10, 1000);

  for (int i = 0; i < 100; i++) {

    int n = dist_n(Random::engine);
    int k = dist_k(Random::engine);
    int c = dist_c(Random::engine);

    bit_vector_t solution(n);
    bv_random(solution);

    MaxSat function;
    function.random(solution, k, c);

    for (int j = 0; j < 1000; j++) {
      if (function.eval(solution) != double(c))
        exit(1);
    }

  }

  return 0;
}
