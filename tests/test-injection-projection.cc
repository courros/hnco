/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#include "hnco/bit-vector.hh"
#include "hnco/map.hh"
#include "hnco/permutation.hh"
#include "hnco/random.hh"

using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Random::generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

  for (int i = 0; i < 1000; i++) {

    std::uniform_int_distribution<int> input_size_dist(1, 100);
    std::uniform_int_distribution<int> delta_size_dist(0, 100);

    int input_size = input_size_dist(Random::generator);
    int delta_size = delta_size_dist(Random::generator);
    int output_size = input_size + delta_size;

    permutation_t permutation(output_size);
    perm_random(permutation);
    std::vector<int> bit_positions(input_size);
    for (std::size_t i = 0; i < bit_positions.size(); i++)
      bit_positions[i] = permutation[i];

    Injection injection(bit_positions, output_size);
    Projection projection(bit_positions, output_size);

    bit_vector_t a(input_size);
    bit_vector_t b(output_size);
    bit_vector_t c(input_size);

    bv_random(a);
    bv_random(b);

    injection.map(a, b);
    projection.map(b, c);

    if (c != a)
      return 1;
  }

  return 0;
}
