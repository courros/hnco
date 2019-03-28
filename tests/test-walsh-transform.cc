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

#include "hnco/random.hh"
#include "hnco/functions/all.hh"

using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


int main(int argc, char *argv[])
{
  Random::engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

  std::uniform_int_distribution<int> bv_size_dist(1, 10); 

  for (int i = 0; i < 100; i++) {
    int bv_size = bv_size_dist(Random::engine);
    int norm = 1 << bv_size;
    Plateau source(bv_size);
    std::vector<Function::WalshTransformTerm> terms;
    source.compute_walsh_transform(terms);
    WalshExpansion destination;
    destination.set_terms(terms);
    bit_vector_t bv(bv_size);
    for (int t = 0; t < 1000; t++) {
      bv_random(bv);
      if (norm * destination.eval(bv) != source.eval(bv))
        return 1;
    }
  }

  return 0;
}
