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
#include <iostream>
#include <iterator>

#include "hnco/bit-matrix.hh"


using namespace hnco::random;
using namespace hnco;
using namespace std;

bool check_bm_swap_rows()
{
  std::uniform_int_distribution<int> dimension_dist(1, 100);
  for (size_t t = 0; t < 1000; t++) {
    int dimension = dimension_dist(Random::engine);
    bit_matrix_t M(dimension, bit_vector_t(dimension));
    for (size_t i = 0; i < M.size(); i++)
      bv_random(M[i]);
    bit_matrix_t N = M;
    std::uniform_int_distribution<int> index_dist(0, dimension - 1);
    int i = index_dist(Random::engine);
    int j = index_dist(Random::engine);
    bm_swap_rows(M, i, j);
    bm_swap_rows(M, i, j);
    if (M != N)
      return false;
  }
  return true;
}

int main(int argc, char *argv[])
{
  Random::engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

  if (check_bm_swap_rows())
    exit(0);
  else
    exit(1);
}
