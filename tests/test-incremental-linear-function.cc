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

#include <random>

#include "hnco/neighborhoods/neighborhood.hh"
#include "hnco/functions/linear-function.hh"
#include "hnco/random.hh"

using namespace hnco::neighborhood;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  const int num_runs            = 100;
  const int num_iterations      = 100;

  Generator::set_seed();

  std::uniform_int_distribution<int> bv_size_dist(1, 100);
  std::uniform_int_distribution<int> coefficient_dist(-100, 100);

  auto fn = [coefficient_dist]() mutable
    { return coefficient_dist(Generator::engine); };

  for (int i = 0; i < num_runs; i++) {
    int bv_size = bv_size_dist(Generator::engine);
    LinearFunction function;
    function.generate(bv_size, fn);
    StandardBitMutation neighborhood(bv_size);
    bit_vector_t bv(bv_size);
    bv_random(bv);
    neighborhood.set_origin(bv);
    for (int j = 0; j < num_iterations; j++) {
      double value = function.evaluate(neighborhood.get_origin());
      neighborhood.propose();
      if (function.evaluate(neighborhood.get_candidate()) !=
          function.evaluate_incrementally(neighborhood.get_origin(),
                                          value,
                                          neighborhood.get_flipped_bits()))
        return 1;
      neighborhood.keep();
    }
  }

  return 0;
}
