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
#include <iostream>

#include <hnco/algorithms/ea/mu-plus-lambda-ea.hh>
#include "hnco/functions/decorators/function-controller.hh"
#include "hnco/functions/theory.hh"
#include "hnco/random.hh"

using namespace hnco;
using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::function::controller;
using namespace hnco::random;


int main()
{
  Random::generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

  const int bv_size = 50;

  OneMax one_max(bv_size);
  OnBudgetFunction fn(&one_max, 1000);

  MuPlusLambdaEa ea(bv_size, 10, 1);
  ea.set_function(&fn);
  ea.init();

  try { ea.maximize(); }
  catch (LastEvaluation) {}

  point_value_t solution = ea.get_solution();
  bv_display(solution.first, std::cout);
  std::cout << std::endl;
  std::cout << solution.second << std::endl;

  return 0;
}
