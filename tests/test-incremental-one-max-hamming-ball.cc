/* Copyright (C) 2016, 2017 Arnaud Berny

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

#include "hnco/algorithms/ls/neighborhood.hh"
#include "hnco/functions/theory.hh"
#include "hnco/random.hh"

using namespace hnco::neighborhood;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  const int num_runs            = 100;
  const int num_iterations      = 100;

  std::uniform_int_distribution<int> dist_bv_size(1, 100);

  Random::engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

  for (int i = 0; i < num_runs; i++) {
    int bv_size = dist_bv_size(Random::engine);
    std::uniform_int_distribution<int> dist_radius(1, bv_size);
    int radius = dist_radius(Random::engine);

    OneMax function(bv_size);
    HammingBall neighborhood(bv_size, radius);
    bit_vector_t bv(bv_size);

    bv_random(bv);
    neighborhood.set_origin(bv);
    for (int j = 0; j < num_iterations; j++) {
      double value = function.eval(neighborhood.get_origin());
      neighborhood.propose();
      if (function.eval(neighborhood.get_candidate()) !=
          function.eval(neighborhood.get_origin(), value, neighborhood.get_flipped_bits()))
        return 1;
      neighborhood.keep();
    }
  }

  return 0;
}
