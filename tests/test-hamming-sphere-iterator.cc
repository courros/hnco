/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#include "hnco/random.hh"
#include "hnco/neighborhoods/neighborhood-iterator.hh"

using namespace hnco::neighborhood;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Generator::set_seed();

  std::uniform_int_distribution<int> radius_dist(1, 5); 
  std::uniform_int_distribution<int> delta_dist(0, 5); 

  for (int i = 0; i < 1000; i++) {

    int radius = radius_dist(Generator::engine);
    int bv_size = radius + delta_dist(Generator::engine);

    bit_vector_t bv(bv_size);
    bv_random(bv);

    HammingSphereIterator iterator(bv_size, radius);
    iterator.set_origin(bv);

    int count = 0;
    while (iterator.has_next()) {
      if (bv_hamming_distance(iterator.next(), bv) != radius)
        return 1;
      count++;
    };

    // We should compute the size of the neighborhood in the general
    // case, that is (bv_size choose radius). Only implemented the
    // case radius == 2

    if (radius == 2 && count != bv_size * (bv_size - 1) / 2)
      return 1;
  }

  return 0;
}
