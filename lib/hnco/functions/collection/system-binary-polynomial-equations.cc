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

#include <algorithm>            // std::sort
#include <fstream>              // std:ifstream, std::ofstream
#include <sstream>              // std::istringstream
#include <unordered_map>

#include "hnco/app/parser.hh"   // hnco::app::split_string
#include "hnco/permutation.hh"  // hnco::permutation_t
#include "hnco/random.hh"

#include "system-binary-polynomial-equations.hh"

using namespace hnco::app;
using namespace hnco::function;
using namespace hnco::random;

static
std::vector<std::string>
parse_term(std::string str)
{
  std::vector<std::string> variables;
  std::istringstream stream(str);

  for (std::string variable; stream >> variable;)
    variables.push_back(variable);

  return variables;
}

static
void
random_subset(std::vector<int>& subset, int n, int k)
{
  assert(n >= 0);
  assert(k >= 0);
  assert(k <= n);

  int cardinal = n;
  for (int i = 0; i < n; i++) {
    if (k == 0)
      break;
    assert(k > 0);
    assert(cardinal > 0);
    double p = double(k) / double(cardinal);
    if (Generator::uniform() < p) {
      subset.push_back(i);
      k--;
    }
    cardinal--;
  }
}

void
SystemBinaryPolynomialEquations::load(std::string path)
{
  std::ifstream stream(path);
  if (!stream.good())
    throw std::runtime_error("SystemBinaryPolynomialEquations::load: Cannot open " + path);

  _system.clear();
  _variable_names.clear();

  std::unordered_map<std::string, int> indices;

  while (!stream.eof()) {
    std::string line;
    std::getline(stream, line);
    if (line.empty())
      continue;
    // Comment
    if (line[0] == '#')
      continue;
    equation_t equation;
    auto ts = split_string(line, "+");
    for (auto t : ts) {
      term_t term;
      auto vs = parse_term(t);
      if (vs.empty())
        continue;
      if (!(vs.size() == 1 && vs[0] == "1")) {
        for (auto v : vs) {
          if (indices.count(v)) {
            term.push_back(indices[v]);
          } else {
            indices[v] = _variable_names.size();
            _variable_names.push_back(v);
            term.push_back(indices[v]);
          }
        }
      }
      equation.push_back(term);
    }
    _system.push_back(equation);
  }

  _is_satisfied.resize(_system.size());
}

void
SystemBinaryPolynomialEquations::save(std::string path) const
{
  std::ofstream stream(path);
  if (!stream.good())
    throw std::runtime_error("SystemBinaryPolynomialEquations::save: Cannot open " + path);

  for (auto& equation : _system) {
    for (std::size_t j = 0; j < equation.size(); j++) {
      auto& term = equation[j];
      if (term.size() == 0)
        stream << "1";
      else {
        for (std::size_t v = 0; v < term.size(); v++) {
          stream << _variable_names[term[v]];
          if (v + 1 != term.size())
            stream << " ";
        }
      }
      if (j + 1 != equation.size())
        stream << " + ";
    }
    stream << std::endl;
  }
}

int
SystemBinaryPolynomialEquations::get_degree() const
{
  int degree = 0;
  for (auto& equation : _system) {
    for (auto& term : equation) {
      if (int(term.size()) > degree)
        degree = term.size();
    }
  }
  return degree;
}

void
SystemBinaryPolynomialEquations::display(std::ostream& stream) const
{
  Function::display(stream);
  stream << "SystemBinaryPolynomialEquations: "
         << _system.size() << " equations, "
         << _variable_names.size() << " variables, "
         << "degree " << get_degree()
         << std::endl;
  for (auto& equation : _system) {
    for (std::size_t j = 0; j < equation.size(); j++) {
      auto& term = equation[j];
      if (term.size() == 0)
        stream << "1";
      else {
        for (std::size_t v = 0; v < term.size(); v++) {
          stream << _variable_names[term[v]];
          if (v + 1 != term.size())
            stream << ".";
        }
      }
      if (j + 1 != equation.size())
        stream << " + ";
    }
    stream << " = 0" << std::endl;
  }
}

void
SystemBinaryPolynomialEquations::describe(const bit_vector_t& bv, std::ostream& stream)
{
  assert(bv.size() == _variable_names.size());

  permutation_t permutation(bv.size());
  perm_identity(permutation);
  std::sort(begin(permutation), end(permutation), [this](int i, int j){ return _variable_names[i] < _variable_names[j]; });

  for (std::size_t i = 0; i < bv.size(); i++) {
    int index = permutation[i];
    stream << _variable_names[index] << " = " << int(bv[index]) << std::endl;
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
  std::uniform_int_distribution<int> num_terms_dist(1, num_terms_max);
  std::uniform_int_distribution<int> d_dist(0, degree);
  term_t term;

  for (int j = 0; j < num_equations; j++) {
    equation_t equation;
    int num_terms = num_terms_dist(Generator::engine);
    for (int t = 0; t < num_terms; t++) {
      do {
        term.clear();
        random_subset(term, num_variables, d_dist(Generator::engine));
      } while ((std::find(begin(equation), end(equation), term) != equation.end()) || // already sampled
               (num_terms == 1 && term.size() == 0)); // avoid equation 1 = 0
      equation.push_back(term);
      for (auto v : term)
        is_used[v] = 1;
    }
    // Sort by decreasing degree
    std::sort(begin(equation), end(equation), [](term_t& t1, term_t& t2) { return t1.size() > t2.size(); });
    _system.push_back(equation);
  }

  _variable_names.resize(num_variables);
  for (int i = 0; i < num_variables; i++)
    _variable_names[i] = "x" + std::to_string(i + 1);

  _is_satisfied.resize(_system.size());
}

double
SystemBinaryPolynomialEquations::evaluate(const bit_vector_t& bv)
{
  assert(bv.size() == _variable_names.size());

  bv_clear(_is_satisfied);
  for (std::size_t j = 0; j < _system.size(); j++) {
    auto& equation = _system[j];
    std::size_t count = 0;
    for (auto& term : equation) {
      bool all_ones = true;
      for (std::size_t v = 0; v < term.size(); v++) {
        assert(is_in_range(term[v], bv.size()));
        if (bv[term[v]] == 0) {
          all_ones = false;
          break;
        }
      }
      if (all_ones)
        count++;
    }
    if (count % 2 == 0)
      _is_satisfied[j] = 1;
  }

  return bv_hamming_weight(_is_satisfied);
}
