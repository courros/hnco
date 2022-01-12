/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#include "hnco/functions/collection/ising/nearest-neighbor-ising-model-2.hh"
#include "hnco/random.hh"

using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Generator::set_seed();

  const std::string path("test-serialize-nn-ising-2.txt");

  for (int i = 0; i < 10; i++) {

    std::uniform_int_distribution<int> dist_num_rows(1, 100);
    std::uniform_int_distribution<int> dist_num_columns(1, 100);

    int num_rows = dist_num_rows(Generator::engine);
    int num_columns = dist_num_columns(Generator::engine);

    NearestNeighborIsingModel2 src;
    src.random(num_rows, num_columns);
    {
      std::ofstream ofs(path);
      boost::archive::text_oarchive oa(ofs);
      oa << src;
    }

    NearestNeighborIsingModel2 dest;
    {
      std::ifstream ifs(path);
      if (!ifs.good())
        return 1;
      boost::archive::text_iarchive ia(ifs);
      ia >> dest;
    }

    bit_vector_t bv(num_rows * num_columns);

    for (int j = 0; j < 1000; j++) {
      bv_random(bv);
      if (src.evaluate(bv) != dest.evaluate(bv))
        return 1;
    }

  }

  return 0;
}
