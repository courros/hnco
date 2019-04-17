/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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
#include "hnco/functions/factorization.hh"
#include "hnco/random.hh"

using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Random::generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

  std::uniform_int_distribution<int> dist_a(2, 15);
  std::uniform_int_distribution<int> dist_b(2, 15);

  for (int i = 0; i < 10; i++) {

    int a = dist_a(Random::generator);
    int b = dist_b(Random::generator);
    int c = a * b;

    std::ostringstream stream;
    stream << c;

    Factorization function0(stream.str());

    Translation map;
    map.random(function0.get_bv_size());

    FunctionMapComposition function(&function0, &map);

    CompleteSearch algorithm(function0.get_bv_size());
    algorithm.set_function(&function);
    algorithm.init();
    try { algorithm.maximize(); }
    catch (...) {
      return 1;
    }

    if (algorithm.get_solution().second != double(0))
      return 1;
  }

  return 0;
}