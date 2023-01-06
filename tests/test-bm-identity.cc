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


bool check_bm_identity()
{
  std::uniform_int_distribution<int> dimension_dist(1, 100);
  for (size_t t = 0; t < 1000; t++) {
    int dimension = dimension_dist(Generator::engine);
    bit_matrix_t M(dimension, bit_vector_t(dimension));
    bm_identity(M);
    if (!bm_is_identity(M))
      return false;
  }
  return true;
}

int main(int argc, char *argv[])
{
  Generator::set_seed();

  if (check_bm_identity())
    exit(0);
  else
    exit(1);

}
