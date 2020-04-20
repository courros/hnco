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

/** \file

    Check ts_random_disjoint.

    Check that all transvections in the sequence are pairwise
    disjoint.
*/

#include <chrono>
#include <iostream>
#include <iterator>

#include "hnco/transvection.hh"


using namespace hnco::random;
using namespace hnco;
using namespace std;

bool check_ts(const transvection_sequence_t& ts)
{
  for (size_t i = 0; i < ts.size(); i++)
    for (size_t j = i + 1; j < ts.size(); j++)
      if (!transvections_are_disjoint(ts[i], ts[j]))
        return false;
  return true;
}

bool check()
{
  uniform_int_distribution<int> dist(2, 200);

  for (int i = 0; i < 10; i++) {
    const int n = dist(Random::generator);

    transvection_sequence_t ts;
    ts_random_disjoint(ts, n, n / 2);
    if (!check_ts(ts))
      return false;
  }

  return true;
}

int main(int argc, char *argv[])
{
  Random::generator.seed(std::chrono::system_clock::now().time_since_epoch().count());

  if (check())
    return 0;
  else
    return 1;

}
