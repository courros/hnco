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

/** \file

    Check ts_random_unique_source.

    Check that the transvection_sequence_t sampled by
    ts_random_unique_source is an involution.
*/

#include <iostream>
#include <iterator>

#include "hnco/transvection.hh"


using namespace hnco::random;
using namespace hnco;


bool check_involution()
{
  std::uniform_int_distribution<int> dist(2, 1000);

  for (int i = 0; i < 10; i++) {
    const int n = dist(Generator::engine);

    transvection_sequence_t ts;
    ts_random_unique_source(ts, n, n - 1);

    for (int j = 0; j < 10; j++) {
      bit_vector_t x(n);
      bit_vector_t y(n);

      bv_random(x);
      y = x;
      ts_multiply(y, ts);
      ts_multiply(y, ts);

      if (y != x)
        return false;
    }

  }

  return true;
}

int main(int argc, char *argv[])
{
  Generator::set_seed();

  if (check_involution())
    return 0;
  else
    return 1;

}
