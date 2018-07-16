/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#include "hnco/random.hh"
#include "hnco/neighborhoods/neighborhood-iterator.hh"

using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Random::engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

  std::uniform_int_distribution<int> bv_size_dist(0, 100); 

  for (int i = 0; i < 100; i++) {
    int bv_size = bv_size_dist(Random::engine);
    HammingSphereIterator iterator(bv_size, 2); // radius should also be random instead of 2
    bit_vector_t bv(bv_size);
    bv_random(bv);
    iterator.set_origin(bv);
    int count = 0;
    while (iterator.has_next()) {
      if (bv_hamming_distance(iterator.next(), bv) != 2)
        return 1;
      count++;
    };
    if (count != bv_size * (bv_size - 1) / 2)
      return 1;
  }

  return 0;
}
