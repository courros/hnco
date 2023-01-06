/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#include <assert.h>

#include <cmath>                // std::abs
#include <limits>               // std::numeric_limits
#include <utility>              // std::swap

#include "hnco/bit-vector.hh"
#include "hnco/representations/integer.hh"

using namespace hnco::random;
using namespace hnco::representation;
using namespace hnco;


inline bool property(double x, double a, double b)
{
  return (a <= x) && (x <= b);
}

bool check_representation()
{
  std::uniform_int_distribution<int> dist_num_variables(1, 100);
  std::uniform_int_distribution<long> dist_bound(std::numeric_limits<long>::min(),
                                                 std::numeric_limits<long>::max());

  const int num_variables = dist_num_variables(Generator::engine);

  long a;
  long b;
  do {
    a = dist_bound(Generator::engine);
    b = dist_bound(Generator::engine);
  } while (!((a < b) && difference_is_safe(a, b)));

  using Rep = DyadicIntegerRepresentation<long>;

  std::vector<Rep> reps(num_variables, Rep(a, b));
  std::vector<double> variables(num_variables);

  const int num_bits = reps[0].size();

  bit_vector_t bv(num_variables * num_bits);
  bv_random(bv);

  int start = 0;
  for (int i = 0; i < num_variables; i++) {
    variables[i] = reps[i].unpack(bv, start);
    if (!property(variables[i], a, b))
      return false;
    start += reps[i].size();
  }

  return true;
}


bool check()
{
  for (int i = 0; i < 100; i++)
    if (!check_representation())
      return false;
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
