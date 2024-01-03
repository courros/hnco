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

#include "hnco/bit-vector.hh"
#include "hnco/random.hh"

using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Generator::set_seed();

  std::uniform_int_distribution<int> bv_size_dist(0, 64);

  for (int i = 0; i < 1000; i++) {

    const int bv_size = bv_size_dist(Generator::engine);

    bit_vector_t src(bv_size);
    bv_random(src);
    std::size_t index = bv_to_size_type(src);

    bit_vector_t dest(bv_size);
    bv_from_size_type(dest, index);

    if (dest != src)
      return 1;
  }

  return 0;
}
