/* Copyright (C) 2016 Arnaud Berny

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

#include "hnco/algorithms/ls/local-search.hh"
#include "hnco/functions/theory.hh"
#include "hnco/random.hh"

using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;
using namespace std;


int main(int argc, char *argv[])
{

  for (int i = 0; i < 1000; i++) {

    uniform_int_distribution<int> choose_size(1, 100);
    int bv_size = choose_size(random::Random::engine);

    SingleBitFlipIterator neighborhood(bv_size);
    OneMax function(bv_size);
    SteepestAscentHillClimbing algo(bv_size, &neighborhood);

    algo.set_function(&function);
    algo.init();

    try { algo.maximize(); }
    catch (LocalMaximum) {}
    catch (...) {
      exit(1);
    }

    if (bv_hamming_weight(algo.get_solution()) != bv_size) {
      exit(1);
    }
  }

  exit(0);
}
