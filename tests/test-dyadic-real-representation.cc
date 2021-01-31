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

#include <cmath>                // std::abs

#include "hnco/bit-vector.hh"
#include "hnco/functions/representations/representation.hh"

using namespace hnco::random;
using namespace hnco;
using namespace hnco::function::representation;


inline bool property(double x, double a, double b)
{
  return (a <= x) && (x < b);
}

bool check_representation()
{
  std::uniform_int_distribution<int> dist_num_variables(1, 100);
  std::uniform_int_distribution<int> dist_num_bits(1, 100);

  const int num_variables = dist_num_variables(Generator::engine);
  const int num_bits = dist_num_bits(Generator::engine);
  const int n = num_variables * num_bits;

  const double scale = 1 + 10 * std::abs(Generator::normal());
  const double lower_bound = scale * Generator::normal();
  const double upper_bound = lower_bound + scale * (1 + Generator::uniform());

  using Rep = DyadicRealRepresentation<double>;

  std::vector<Rep> reps(num_variables, Rep(num_bits, lower_bound, upper_bound));
  std::vector<double> variables(num_variables);

  bit_vector_t bv(n);
  bv_random(bv);

  int start = 0;
  for (int i = 0; i < num_variables; i++) {
    variables[i] = reps[i].unpack(bv, start);
    if (!property(variables[i], lower_bound, upper_bound))
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
