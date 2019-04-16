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

#include "hnco/functions/ising/nearest-neighbor-ising-model-1.hh"
#include "hnco/random.hh"

using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Random::generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

  const std::string path("test-serialize-nn-ising-1.txt");

  for (int i = 0; i < 10; i++) {

    std::uniform_int_distribution<int> dist_n(1, 100);
    int n = dist_n(Random::generator);

    NearestNeighborIsingModel1 src;
    src.random(n);
    {
      std::ofstream ofs(path);
      boost::archive::text_oarchive oa(ofs);
      oa << src;
    }

    NearestNeighborIsingModel1 dest;
    {
      std::ifstream ifs(path);
      if (!ifs.good())
        exit(1);
      boost::archive::text_iarchive ia(ifs);
      ia >> dest;
    }

    bit_vector_t bv(n);

    for (int j = 0; j < 1000; j++) {
      bv_random(bv);
      if (src.eval(bv) != dest.eval(bv))
        exit(1);
    }

  }

  return 0;
}