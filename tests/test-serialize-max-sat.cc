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

#include <fstream>              // std::ifstream, std::ofstream

#include "hnco/exception.hh"
#include "hnco/functions/collection/max-sat.hh"
#include "hnco/random.hh"

using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Generator::set_seed();

  const std::string path("test-serialize-max-sat.txt");

  std::uniform_int_distribution<int> dist_n(3, 100);

  for (int i = 0; i < 10; i++) {

    int n = dist_n(Generator::engine);

    MaxSat src;
    {
      src.random(n, 3, 100);
      std::ofstream ofs(path);
      src.save(ofs);
    }

    MaxSat dest;
    {
      std::ifstream ifs(path);
      if (!ifs.good())
        exit(1);
      try { dest.load(ifs); }
      catch (Error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
      }
    }

    bit_vector_t bv(n);

    for (int j = 0; j < 1000; j++) {
      bv_random(bv);
      if (src.evaluate(bv) != dest.evaluate(bv))
        exit(1);
    }

  }

  return 0;
}
