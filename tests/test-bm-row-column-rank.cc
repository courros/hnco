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

#include "hnco/bit-matrix.hh"


using namespace hnco::random;
using namespace hnco;


bool check_bm_row_column_rank()
{
  std::uniform_int_distribution<int> dist_dimension(1, 100);

  for (int t = 0; t < 100; t++) {

    const int dimension = dist_dimension(Generator::engine);

    bit_matrix_t M;
    bm_resize(M, dimension);
    bm_random(M);

    bit_matrix_t N = M;
    bm_row_echelon_form(N);
    int row_rank = bm_rank(N);

    N = bm_transpose(M);
    bm_row_echelon_form(N);
    int column_rank = bm_rank(N);

    if (column_rank != row_rank)
      return false;
  }

  return true;
}

int main(int argc, char *argv[])
{
  Generator::set_seed();

  if (check_bm_row_column_rank())
    return 0;
  else
    return 1;
}
