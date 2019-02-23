/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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
#include <iostream>
#include <iterator>

#include "hnco/bit-matrix.hh"


using namespace hnco::random;
using namespace hnco;
using namespace std;

bool check_bm_rank()
{
  std::uniform_int_distribution<size_t> dimension_dist(1, 100);
  for (size_t t = 0; t < 100; t++) {
    size_t dimension = dimension_dist(Random::engine);

    bit_matrix_t M;
    bm_resize(M, dimension);
    bm_random(M);

    bit_matrix_t N = M;
    bm_row_echelon_form(N);
    size_t rank = bm_rank(N);

    if (bm_invert(M, N) != (rank == dimension))
      return false;
  }
  return true;
}

int main(int argc, char *argv[])
{
  Random::engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

  if (check_bm_rank())
    exit(0);
  else
    exit(1);
}
