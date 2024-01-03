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

#include <assert.h>

#include "hnco/bit-matrix.hh"


using namespace hnco::random;
using namespace hnco;


bool check_bm_add_rows()
{
  std::uniform_int_distribution<int> dist_dimension(2, 100);

  for (size_t t = 0; t < 1000; t++) {

    const int dimension = dist_dimension(Generator::engine);

    bit_matrix_t M;
    bm_resize(M, dimension);
    bm_random(M);

    bit_matrix_t N = M;

    std::uniform_int_distribution<int> dist_index(0, dimension - 1);

    int i, j;
    do {
      i = dist_index(Generator::engine);
      j = dist_index(Generator::engine);
    } while (i == j);
    assert(i != j);

    bm_add_columns(M, i, j);
    bm_add_columns(M, i, j);

    if (M != N)
      return false;
  }

  return true;
}

int main(int argc, char *argv[])
{
  Generator::set_seed();

  if (check_bm_add_rows())
    return 0;
  else
    return 1;
}
