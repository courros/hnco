/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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

    Check bv_from_stream.

*/

#include <chrono>
#include <iostream>
#include <iterator>
#include <fstream>              // std::ifstream, std::ofstream

#include "hnco/bit-vector.hh"
#include "hnco/random.hh"


using namespace hnco::random;
using namespace hnco;


bool check()
{
  const std::string path("test-bv-from-stream.txt");

  std::uniform_int_distribution<int> dist(0, 1000);

  for (int i = 0; i < 100; i++) {
    const int n = dist(Generator::engine);

    bit_vector_t x(n);
    bv_random(x);
    {
      std::ofstream stream(path);
      bv_display(x, stream);
    }

    bit_vector_t y(n);
    {
      std::ifstream stream(path);
      bv_from_stream(y, stream);
    }

    if (y != x)
      return false;
  }

  return true;
}

int main(int argc, char *argv[])
{
  Generator::set_seed();

  if (check())
    return 0;
  else
    return 1;

}
