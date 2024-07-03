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

/** @file
 *
 * Check TsAffineMap::invert.
 *
 * Check that the composition of a TsAffineMap and its inverse is the
 * identity.
 */

#include "hnco/maps/map.hh"

using namespace hnco::map;
using namespace hnco::random;
using namespace hnco;

bool
check_identity()
{
  std::uniform_int_distribution<int> n_dist(2, 1000);
  std::uniform_int_distribution<int> t_dist(0, 1000);

  for (int i = 0; i < 100; i++) {
    const int n = n_dist(Generator::engine);
    const int t = t_dist(Generator::engine);
    TsAffineMap map;
    TsAffineMap inverse_map;
    map.random(n, t, TsAffineMap::SamplingMode::unconstrained);
    inverse_map = map;
    inverse_map.invert();
    for (int j = 0; j < 10; j++) {
      bit_vector_t bv1(n);
      bit_vector_t bv2(n);
      bit_vector_t bv3(n);
      bv_random(bv1);
      map.map(bv1, bv2);
      inverse_map.map(bv2, bv3);
      if (bv3 != bv1)
        return false;
    }
  }
  return true;
}

int
main(int argc, char *argv[])
{
  Generator::set_seed();
  if (check_identity())
    return 0;
  else
    return 1;
}
