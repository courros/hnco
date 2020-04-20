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

    Check ts_random_non_commuting.

    Check that all consecutive transvections in the sequence do not
    commute.
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
  for (size_t i = 1; i < ts.size(); i++)
    if (transvections_commute(ts[i], ts[i - 1]))
      return false;
  return true;
}

bool check()
{
  uniform_int_distribution<int> dist(2, 200);

  for (int i = 0; i < 10; i++) {
    const int n = dist(Random::generator);
    const int t = 2 * n;

    transvection_sequence_t ts;
    ts_random_non_commuting(ts, n, t);
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
