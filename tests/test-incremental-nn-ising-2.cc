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
#include "hnco/functions/ising/nearest-neighbor-ising-model-2.hh"
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

  std::uniform_int_distribution<int> dist_num_rows(1, 20);
  std::uniform_int_distribution<int> dist_num_columns(1, 20);
  std::uniform_int_distribution<int> dist_coefficient(-100, 100);

  auto generator = [dist_coefficient]() mutable
    {
      return dist_coefficient(Generator::engine);
    };

  for (int i = 0; i < num_runs; i++) {
    int num_rows = dist_num_rows(Generator::engine);
    int num_columns = dist_num_columns(Generator::engine);
    int bv_size = num_rows * num_columns;

    NearestNeighborIsingModel2 function;
    function.generate(num_rows, num_columns, generator, generator);
    if (Generator::bernoulli())
      function.set_periodic_boundary_conditions(true);

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
