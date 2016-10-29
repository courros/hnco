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

#include <algorithm>            // all_of

#include "hnco/bit-vector.hh"
#include "hnco/iterator.hh"

using namespace hnco::random;
using namespace hnco;
using namespace std;


int main(int argc, char *argv[])
{
  const int bv_size = 20;

  HypercubeIterator iterator(bv_size);
  iterator.init();
  while (iterator.has_next()) {
    iterator.next();
  };

  const bit_vector_t& bv = iterator.get_current();

  if (all_of(bv.begin(), bv.end(), [](bit_t i){ return i == 1; }))
    return 0;
  else
    return 1;
}
