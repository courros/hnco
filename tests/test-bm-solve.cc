/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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
#include <iterator>

#include "hnco/bit-matrix.hh"


using namespace hnco::random;
using namespace hnco;


bool check_bm_solve()
{
  std::uniform_int_distribution<int> dimension_dist(1, 100);
  for (size_t t = 0; t < 100; t++) {
    int dimension = dimension_dist(Generator::engine);

    bit_matrix_t A, B, C;
    bm_resize(A, dimension);
    bm_resize(B, dimension);

    do {
      bm_random(A);
      C = A;
    } while (!bm_invert(C, B));
    // B = inv(A)

    if (!bm_is_identity(C))
      return false;

    bit_vector_t b, c, d;
    b.resize(dimension);
    d.resize(dimension);
    bv_random(b);

    // Solve Ax = b for x
    C = A;
    c = b;
    bm_solve(C, c);

    // d = inv(A) b
    bm_multiply(d, B, b);

    if (c != d)
      return false;
  }
  return true;
}

int main(int argc, char *argv[])
{
  Generator::set_seed();

  return check_bm_solve() ? 0 : 1;
}
