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

#include "hnco/transvection.hh"


using namespace hnco::random;
using namespace hnco;
using namespace std;

bool check_ts_multiply()
{
  uniform_int_distribution<int> dimension_dist(2, 10);
  uniform_int_distribution<int> length_dist(0, 10);

  for (int i = 0; i < 10; i++) {
    const int n = dimension_dist(Random::generator);
    const int t = length_dist(Random::generator);

    bit_matrix_t M;
    bm_identity(M, n);

    transvection_sequence_t seq;
    ts_random(seq, n, t);
    ts_multiply(seq, M);

    for (int j = 0; j < 10; j++) {
      bit_vector_t x(n);
      bit_vector_t y(n);
      bv_random(x);
      bv_random(y);
      bm_multiply(M, x, y);

      bit_vector_t z(x);
      ts_multiply(seq, z);

      if (z != y)
        return false;
    }

  }

  return true;
}

int main(int argc, char *argv[])
{
  Random::generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

  if (check_ts_multiply())
    return 0;
  else
    return 1;

}