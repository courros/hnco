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

#include "hnco/algorithms/ls/neighborhood.hh"
#include "hnco/random.hh"

using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  const int num_iterations = 1000;
  const int bv_size_max = 10;

  std::uniform_int_distribution<int> dist(1, bv_size_max);

  Random::engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

  for (int i = 0 ; i < num_iterations; i++) {

    int n, radius;

    do {
      n = dist(Random::engine);
      radius = dist(Random::engine);
    } while (radius > n);

    assert(radius <= n);

    bit_vector_t bv(n);
    bv_random(bv);

    HammingSphere neighborhood(n, radius);
    neighborhood.set_origin(bv);

    neighborhood.propose();
    if (bv_hamming_distance(neighborhood.get_origin(),
                            neighborhood.get_candidate()) != radius)
      return 1;
  }

  return 0;
}
