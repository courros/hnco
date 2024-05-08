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
#include "hnco/algorithms/walsh-moment/walsh-moment.hh"

using namespace hnco::algorithm::walsh_moment;
using namespace hnco::random;
using namespace hnco;

bool check()
{
  using Dist = std::uniform_int_distribution<int>;
  using TM = LowerTriangularWalshMoment2;
  using SM = SymmetricWalshMoment2;

  Dist dist_n(1, 100); // bit vector size
  Dist dist_population_size(1, 100); // population size
  for (int c = 0; c < 100; c++) {
    const int n = dist_n(Generator::engine);
    const int population_size = dist_population_size(Generator::engine);
    bit_vector_t bv(n);
    TM triangular(n);
    SM symmetric(n);
    triangular.init();
    symmetric.init();
    for (int k = 0; k < population_size; k++) {
      bv_random(bv);
      triangular.add(bv);
      symmetric.add(bv);
    }
    for (int i = 0; i < n; i++)
      if (triangular.first_moment[i] != symmetric.first_moment[i])
        return false;
    for (int i = 0; i < n; i++)
      for (int j = 0; j < i; j++)
        if (triangular.second_moment[i][j] != symmetric.second_moment[i][j])
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
