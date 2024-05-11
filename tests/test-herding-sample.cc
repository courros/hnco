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
#include "hnco/algorithms/walsh-moment/herding.hh"

using namespace hnco::algorithm::walsh_moment;
using namespace hnco::random;
using namespace hnco;

bool check()
{
  using Dist = std::uniform_int_distribution<int>;
  using TM = LowerTriangularWalshMoment2;
  using TH = LowerTriangularWalshMoment2Herding;
  using SM = SymmetricWalshMoment2;
  using SH = SymmetricWalshMoment2Herding;

  Dist dist_n(1, 100); // bit vector size
  Dist dist_population_size(1, 100); // population size
  Dist dist_sequence_size(1, 100); // sequence size
  for (int c = 0; c < 100; c++) {
    const int n = dist_n(Generator::engine); // bit vector dimension
    const int population_size = dist_population_size(Generator::engine);
    const int sequence_size = dist_sequence_size(Generator::engine);
    bit_vector_t bv(n);
    TM tm(n);
    SM sm(n);
    tm.init();
    sm.init();
    for (int k = 0; k < population_size; k++) {
      bv_random(bv);
      tm.add(bv);
      sm.add(bv);
    }
    tm.average(population_size);
    sm.average(population_size);
    TH th(n);
    SH sh(n);
    bit_vector_t bv_th(n);
    bit_vector_t bv_sh(n);
    th.set_randomize_bit_order(false);
    th.init();
    sh.set_randomize_bit_order(false);
    sh.init();
    for (int k = 0; k < sequence_size; k++) {
      th.sample(tm, bv_th);
      sh.sample(sm, bv_sh);
      if (bv_th != bv_sh)
        return false;
    }
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
