/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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
#include "hnco/functions/modifiers/modifier.hh"
#include "hnco/functions/collection/factorization.hh"
#include "hnco/random.hh"

using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::function::modifier;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Generator::set_seed();

  std::uniform_int_distribution<int> dist_a(2, 15);
  std::uniform_int_distribution<int> dist_b(2, 15);

  for (int i = 0; i < 10; i++) {

    const int a = dist_a(Generator::engine);
    const int b = dist_b(Generator::engine);
    const int c = a * b;

    std::ostringstream stream;
    stream << c;

    Factorization function0(stream.str());
    Translation map;
    map.random(function0.get_bv_size());
    FunctionMapComposition function(&function0, &map);

    CompleteSearch algorithm(function0.get_bv_size());

    try { algorithm.maximize({&function}); }
    catch (...) {
      return 1;
    }

    if (algorithm.get_solution().second != double(0))
      return 1;
  }

  return 0;
}
