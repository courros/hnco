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

#include "hnco/algorithms/ls/steepest-ascent-hill-climbing.hh"
#include "hnco/functions/labs.hh"
#include "hnco/random.hh"

using namespace hnco::algorithm;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Generator::set_seed();

  std::uniform_int_distribution<int> bv_size_dist(1, 100);

  for (int i = 0; i < 1000; i++) {

    const int bv_size = bv_size_dist(random::Generator::engine);

    Labs fn(bv_size);

    SingleBitFlipIterator it(bv_size);
    SteepestAscentHillClimbing algorithm(bv_size, &it);

    try {
      algorithm.maximize({&fn});
    }
    catch (LocalMaximumReached) {}
    catch (...) {
      return 1;
    }

    // finalize not necessary
    if (!bv_is_locally_maximal(algorithm.get_solution().first, fn, it)) {
      return 1;
    }
  }

  return 0;
}
