/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#include "hnco/random.hh"
#include "hnco/sparse-bit-vector.hh"
#include "hnco/sparse-bit-matrix.hh"

using namespace hnco;
using namespace hnco::random;


int main(int argc, char *argv[])
{
  Generator::set_seed();

  const int rows = 100;
  const int cols = 200;

  bit_matrix_t bm;
  bm_resize(bm, rows, cols);

  bit_vector_t x(cols);
  bit_vector_t y(rows);
  bit_vector_t z(rows);

  for (int i = 0; i < 100; i++) {

    bv_random(x);

    bm_random(bm);
    bm_multiply(y, bm, x);

    sparse_bit_matrix_t sbm(rows);
    sbm_from_bm(sbm, bm);
    sbm_multiply(z, sbm, x);

    if (z != y)
      exit(1);
  }

  return 0;
}
