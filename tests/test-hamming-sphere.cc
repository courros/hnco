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

#include <chrono>

#include "hnco/algorithms/ls/neighborhood.hh"
#include "hnco/random.hh"

using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;
using namespace std;


int main(int argc, char *argv[])
{
  const int num_iterations = 1000;
  const int bv_size = 10000;

  Random::engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

  for (int i = 0 ; i < num_iterations; i++) {

    int n, radius;

    do {
      n = 1 + rand() % bv_size;
      radius = 1 + rand() % bv_size;
    } while (radius > n);

    assert(radius <= n);

    bit_vector_t bv(n);
    bv_random(bv);

    HammingSphere sphere(n, radius);
    sphere.set_origin(bv);

    const bit_vector_t& current = sphere.get_origin();
    sphere.propose();
    const bit_vector_t& candidate = sphere.get_candidate();

    if (bv_hamming_distance(current, candidate) != radius) {
      exit(1);
    }

  }

  exit(0);
}
