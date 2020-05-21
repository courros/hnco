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

#include "hnco/algorithms/ea/one-plus-one-ea.hh"
#include "hnco/functions/controllers/controller.hh"
#include "hnco/functions/modifiers/modifier.hh"
#include "hnco/functions/theory.hh"
#include "hnco/random.hh"

using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::function::controller;
using namespace hnco::function::modifier;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Random::generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

  std::uniform_int_distribution<int> bv_size_dist(1, 100);

  for (int i = 0; i < 100; i++) {

    int bv_size = bv_size_dist(Random::generator);

    OneMax f0(bv_size);
    Translation map;
    map.random(bv_size);
    FunctionMapComposition f1(&f0, &map);
    StopOnMaximum f2(&f1);

    OnePlusOneEa algorithm(bv_size);
    algorithm.set_function(&f2);

    point_value_t solution;

    try {
      algorithm.init();
      algorithm.maximize();
    }
    catch (const MaximumReached& e) {
      // finalize not necessary
      solution = e.get_point_value();
    }
    catch (...) {
      return 1;
    }

    if (solution.second != f2.get_maximum())
      return 1;
  }

  return 0;
}
