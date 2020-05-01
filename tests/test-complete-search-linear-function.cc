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
#include "hnco/functions/decorators/function-modifier.hh"
#include "hnco/functions/linear-function.hh"
#include "hnco/random.hh"

using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::function::modifier;
using namespace hnco::random;
using namespace hnco;
using namespace std;


int main(int argc, char *argv[])
{
  Random::generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

  std::uniform_int_distribution<int> bv_size_dist(1, 20);
  std::uniform_int_distribution<int> coefficient_dist(-100, 100);

  auto fn = [coefficient_dist]() mutable
    { return coefficient_dist(Random::generator); };

  for (int i = 0; i < 10; i++) {
    int bv_size = bv_size_dist(Random::generator);

    LinearFunction function0;
    function0.generate(bv_size, fn);

    Translation map;
    map.random(bv_size);

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
