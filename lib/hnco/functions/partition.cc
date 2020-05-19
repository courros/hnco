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

#include <cstdlib>              // std:abs

#include "hnco/random.hh"
#include "hnco/functions/partition.hh"


using namespace hnco::random;
using namespace hnco::function;


double
Partition::eval(const bit_vector_t& s)
{
  assert(s.size() == _numbers.size());

  int sum0 = 0;
  int sum1 = 0;
  for (size_t i = 0; i < s.size(); i++)
    if (s[i])
      sum1 += _numbers[i];
    else
      sum0 += _numbers[i];
  return -std::abs(sum1 - sum0);
}
