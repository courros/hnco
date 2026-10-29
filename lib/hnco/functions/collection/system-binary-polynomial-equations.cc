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
#include <algorithm>            // std::sort

#include "hnco/app/parser.hh"   // hnco::app::split_string
#include "hnco/random.hh"

#include "system-binary-polynomial-equations.hh"

using namespace hnco::app;
using namespace hnco::function;
using namespace hnco::random;

void
SystemBinaryPolynomialEquations::load(std::string path)
{
  std::ifstream stream(path);
  if (!stream.good())
    throw std::runtime_error("SystemBinaryPolynomialEquations::load: Cannot open " + path);

  while (!stream.eof()) {
    std::string line;
    getline(stream, line);

    if (line.empty())
      continue;

    // Comment
    if (line[0] == '#')
      continue;

    // split at +
    auto term_strs = split_string(line, "+");

    // split at space
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
  Function::display(stream);
  stream << "SystemBinaryPolynomialEquations" << std::endl;
  for (auto& equation : _system) {
    for (auto& term : equation) {
      if (term.size() == 0)
        stream << "1";
      else {
        for (auto variable : term) {
          stream << _variable_names[variable] << " ";
        }
      }
      stream << " + ";
    }
    stream << "= 0" << std::endl;
  }
}

static
void
random_subset(std::vector<int>& subset, int n, int k)
{
  assert(k >= 0);
  assert(k <= n);

  for (int i = 0; i < n; i++) {
    if (k > 0) {
      assert(n > 0);
      double p = double(k) / double(n);
      if (Generator::uniform() < p) {
        subset.push_back(i);
        k--;
      }
      n--;
    }
  }
}

void
SystemBinaryPolynomialEquations::random(int num_variables, int num_equations, int degree, int num_terms_max)
{
  assert(num_variables > 0);
  assert(num_equations > 0);
  assert(degree > 0);
  assert(num_terms_max > 0);

  degree = std::min(degree, num_variables);

  bit_vector_t is_used(num_variables, 0);

  for (int j = 0; j < num_equations; j++) {
    equation_t equation;
    std::uniform_int_distribution<int> num_terms_dist(1, num_terms_max);
    int num_terms = num_terms_dist(Generator::engine);
    for (int t = 0; t < num_terms; t++) {
      std::uniform_int_distribution<int> d_dist(0, degree);
      int d = d_dist(Generator::engine);
      term_t term;
      random_subset(term, num_variables, d);
      equation.push_back(term);
      // If only a single term, it must not be the constant 1
      for (auto i : term)
        is_used[i] = 1;
      std::sort(begin(term), end(term));
    }
    // sort by decreasing degree
    _system.push_back(equation);
  }

  _variable_names.resize(num_variables);
  for (int i = 0; i < num_variables; i++)
    _variable_names[i] = "x" + std::to_string(i + 1);
}

double
SystemBinaryPolynomialEquations::evaluate(const bit_vector_t& bv)
{
  double result = 0;

  return result;
}
