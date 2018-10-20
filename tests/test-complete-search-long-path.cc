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
#include <random>

#include "hnco/algorithms/complete-search.hh"
#include "hnco/functions/decorators/function-modifier.hh"
#include "hnco/functions/long-path.hh"
#include "hnco/random.hh"

using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;
using namespace std;


int main(int argc, char *argv[])
{
  Random::engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

  std::uniform_int_distribution<int> prefix_length_dist(2, 20);
  std::uniform_int_distribution<int> divisor_dist(2, 5);

  for (int i = 0; i < 10; i++) {

    int prefix_length = prefix_length_dist(random::Random::engine);
    int bv_size = divisor_dist(random::Random::engine) * prefix_length;

    Translation map;
    map.random(bv_size);

    LongPath function0(bv_size, prefix_length);
    FunctionMapComposition function(&function0, &map);

    CompleteSearch algorithm(bv_size);
    algorithm.set_function(&function);
    algorithm.init();
    try { algorithm.maximize(); }
    catch (...) {
      return 1;
    }

    if (algorithm.get_solution().second != function.get_maximum())
      return 1;
  }

  return 0;
}
