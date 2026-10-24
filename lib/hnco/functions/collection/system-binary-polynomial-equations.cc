/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025, 2026 Arnaud Berny

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

#include <cassert>

#include <iostream>
#include <string>
#include <sstream>
#include <iosfwd>
#include <fstream>              // std:ifstream, std::ofstream

#include "system-binary-polynomial-equations.hh"

#include "hnco/random.hh"

using namespace hnco::function;
using namespace hnco::random;

void
SystemBinaryPolynomialEquations::load(std::string path)
{
  std::ifstream stream(path);
  if (!stream.good())
    throw std::runtime_error("SystemBinaryPolynomialEquations::load: Cannot open " + path);

  // Count variables
  _num_variables = 0;

  while (!stream.eof()) {
    std::string line;
    getline(stream, line);

    if (line.empty())
      continue;

    // Comment
    if (line[0] == '#')
      continue;

  }

}

void
SystemBinaryPolynomialEquations::save(std::string path) const
{
  std::ofstream stream(path);
  if (!stream.good())
    throw std::runtime_error("SystemBinaryPolynomialEquations::save: Cannot open " + path);

}

void
SystemBinaryPolynomialEquations::display(std::ostream& stream) const
{

}

void
SystemBinaryPolynomialEquations::random(int n, int d, int t)
{
  assert(n > 0);
  assert(d > 0);
  assert(t > 0);

  _num_variables = n;
}

double
SystemBinaryPolynomialEquations::evaluate(const bit_vector_t& bv)
{
  double result = 0;

  return result;
}
