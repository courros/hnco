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

#include <assert.h>

#include "hnco/random.hh"

#include "tsp.hh"

using namespace hnco::function;
using namespace hnco::random;


void
Tsp::load_(std::istream& stream)
{
  int line_number = 1;

  // Specification part
  while(!stream.eof()) {
    std::string line;
    getline(stream, line);
    if (line.empty())
      continue;
    std::istringstream iss(line);
    std::string token;
    iss >> token;
    if (token == "NAME") {
      std::cout << "Found NAME!" << std::endl;
    }
    line_number++;
  }
}

void
Tsp::save_(std::ostream& stream) const
{
}


void
Tsp::display(std::ostream& stream) const
{
}


void
Tsp::describe(const hnco::permutation_t& permutation, std::ostream& stream)
{
}


double
Tsp::evaluate(const hnco::permutation_t& permutation)
{
  assert(_distances.size() > 0);
  double result = 0;
  int last = _distances.size() - 1;
  for (int i = 0; i < last; i++)
    result += _distances[permutation[i]][permutation[i+1]];
  result += _distances[permutation[last]][permutation[0]];
  return result;
}
