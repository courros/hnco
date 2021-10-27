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
    auto pos = line.find(":");
    std::string before = line.substr(0, pos);
    std::istringstream before_stream(before);
    std::string key, value;
    before_stream >> key;
    std::string after = line.substr(pos + 1);
    std::istringstream after_stream(after);
    if (key == "NAME") {
      after_stream >> _name;
    } else if (key == "COMMENT") {
      _comment = line.substr(pos + 1);
      // should eat white at the beginning
    } else if (key == "TYPE") {
      after_stream >> value;
      if (value != "TSP")
        throw std::runtime_error("Tsp::load_: " + _name + " is not a TSP problem.");
    } else if (key == "DIMENSION") {
      after_stream >> _num_cities;
    }
    line_number++;
  }

  std::cout << _name << std::endl;
  std::cout << _comment << std::endl;
  std::cout << _num_cities << std::endl;

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
