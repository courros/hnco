/* Copyright (C) 2016 Arnaud Berny

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

#include <chrono>

#include "hnco/random.hh"
#include "hnco/sparse-bit-vector.hh"
#include "hnco/sparse-bit-matrix.hh"

using namespace hnco;
using namespace hnco::random;
using namespace std;


int main(int argc, char *argv[])
{
  Random::engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

  const int rows = 100;
  const int cols = 200;

  bit_matrix_t bm(rows, bit_vector_t(cols));
  bit_vector_t x(cols);
  bit_vector_t y(rows);
  bit_vector_t z(rows);

  for (int i = 0; i < 100; i++) {
    bm_random(bm);
    bv_random(x);
    bm_multiply(bm, x, y);
    sparse_bit_matrix_t sbm;
    bm_to_sbm(bm, sbm);
    sbm_multiply(sbm, x, z);
    if (z != y)
      exit(1);
  }

  return 0;
}
