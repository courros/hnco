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

#include "hnco/maps/transvection.hh"


using namespace hnco::map;
using namespace hnco::random;
using namespace hnco;


bool check_ts_multiply()
{
  std::uniform_int_distribution<int> dist_dimension(2, 10);
  std::uniform_int_distribution<int> dist_length(0, 10);

  for (int i = 0; i < 10; i++) {

    const int n = dist_dimension(Generator::engine);
    const int t = dist_length(Generator::engine);

    transvection_sequence_t seq;
    ts_random(seq, n, t);

    bit_matrix_t M = bm_identity(n);
    ts_multiply(M, seq);

    for (int j = 0; j < 10; j++) {

      bit_vector_t x(n);
      bv_random(x);

      bit_vector_t y(n);
      bm_multiply(y, M, x);

      bit_vector_t z(x);
      ts_multiply(z, seq);

      if (z != y)
        return false;
    }

  }

  return true;
}

int main(int argc, char *argv[])
{
  Generator::set_seed();

  if (check_ts_multiply())
    return 0;
  else
    return 1;
}
