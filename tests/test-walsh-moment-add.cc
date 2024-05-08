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

#include <iostream>

#include "hnco/bit-vector.hh"
#include "hnco/random.hh"
#include "hnco/algorithms/walsh-moment/walsh-moment.hh"

using namespace hnco::algorithm::walsh_moment;
using namespace hnco::random;
using namespace hnco;

bool check()
{
  using Dist = std::uniform_int_distribution<int>;
  Dist dist_n(1, 5); // bit vector size
  LowerTriangularWalshMoment2 triangular(n);
  SymmetricWalshMoment2 symmetric(n);
  Dist dist_exponent(0, 10);
  for (int i = 0; i < 100; i++) {
    size_t population_size = 1 << dist_exponent(Generator::engine); // The population size is a power of 2.
    bit_vector_t bv(n);
    triangular.init();
    symmetric.init();
    for (int k = 0; k < population_size; k++) {
      bv_random(bv);
      triangular.add(bv);
      symmetric.add(bv);
    }
    if (triangular != symmetric)
      return false;
  }
  return true;
}

int main(int argc, char *argv[])
{
  Generator::set_seed();
  if (check())
    return 0;
  else
    return 1;
}
