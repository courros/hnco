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

#include <cstdlib>              // std:abs

#include "hnco/util.hh"              // hnco::join

#include "partition.hh"


using namespace hnco::function;


double
Partition::evaluate(const bit_vector_t& s)
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


void
Partition::display(std::ostream& stream) const
{
  std::vector<int> S = _numbers;
  stream
    << "Partition the multiset S = {"
    << join(begin(S), end(S), ", ")
    << "}\n";
}


void
Partition::describe(const bit_vector_t& x, std::ostream& stream)
{
  assert(int(x.size()) == get_bv_size());

  display(stream);
  std::vector<int> A, B;
  for (size_t i = 0; i < x.size(); i++)
    if (x[i])
      A.push_back(_numbers[i]);
    else
      B.push_back(_numbers[i]);
  stream
    << "A = {"
    << join(begin(A), end(A), ", ")
    << "}\n";
  stream
    << "B = {"
    << join(begin(B), end(B), ", ")
    << "}\n";
}
