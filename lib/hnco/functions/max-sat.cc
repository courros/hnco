/* Copyright (C) 2016 Arnaud Berny

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

#include <iostream>
#include <string>
#include <sstream>

#include "hnco/exception.hh"
#include "hnco/random.hh"

#include "max-sat.hh"

using namespace std;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::random;


void
MaxSat::random(int n, int k, int c)
{
  assert(n > 0);
  assert(k > 0);
  assert(c > 0);
  assert(k <= n);

  _num_variables = n;
  _expression = std::vector<std::vector<int> >(c, std::vector<int>(k));

  bit_vector_t x(n);
  for (size_t i = 0; i < _expression.size(); i++) {
    bv_random(x, k);
    int count = 0;
    for (size_t j = 0; j < x.size(); j++) {
      if (x[j]) {
        if (Random::random_bit())
          _expression[i][count++] = j + 1;
        else
          _expression[i][count++] = -(j + 1);
      }
    }
    assert(count == bv_hamming_weight(x));
  }
}


void
MaxSat::random(const bit_vector_t& solution, int k, int c)
{
  assert(k > 0);
  assert(c > 0);
  assert(size_t(k) <= solution.size());

  _num_variables = solution.size();
  _expression = std::vector<std::vector<int> >(c, std::vector<int>(k));

  bit_vector_t x(_num_variables);
  for (size_t i = 0; i < _expression.size(); i++) {
    bv_random(x, k);
    int count = 0;
    for (size_t j = 0; j < x.size(); j++) {
      if (x[j]) {
        if (Random::random_bit())
          _expression[i][count++] = j + 1;
        else
          _expression[i][count++] = -(j + 1);
      }
    }
    assert(count == bv_hamming_weight(x));

    bool ok = false;
    for (size_t j = 0; j < _expression[i].size(); j++) {
      int lit = _expression[i][j];
      if ((lit > 0 && (solution[lit - 1] == 1)) ||
          (lit < 0 && (solution[-lit - 1] == 0))) {
        ok = true;
        break;
      }
    }
    if (!ok) {
      uniform_int_distribution<int> dist(0, _expression[i].size() - 1);
      int index = dist(Random::engine);
      _expression[i][index] = -_expression[i][index];
    }
  }

  assert(eval(solution) == double(_expression.size()));
}


void
eat_white(istringstream& stream)
{
  if (stream.eof())
    return;

  char c;
  do {
    stream.get(c);
  } while (stream.good() && (c == ' ' || c == '\t'));
  if (stream.good())
    stream.putback(c);
}


bool
has_next_variable(istringstream& stream)
{
  eat_white(stream);
  return stream.good();
}


void
MaxSat::load(istream& stream)
{
  _expression.clear();
  _num_variables = 0;

  bool spec = false;
  size_t num_clauses = 0;

  while (!stream.eof()) {
    string line;
    getline(stream, line);

    if (line.empty())
      continue;

    // Comment
    if (line[0] == 'c')
      continue;

    // Spec
    if (line[0] == 'p') {

      if (spec)
        throw Error("MaxSat::load: More than one p line");

      istringstream iss(line);
      string token;

      iss >> token;
      if (iss.fail() || token != "p")
        throw Error("MaxSat::load: Bad p line");

      iss >> token;
      if (iss.fail() || token != "cnf")
        throw Error("MaxSat::load: Bad p line");

      int n;

      iss >> n;
      if (iss.fail() || n <= 0)
        throw Error("MaxSat::load: Bad number of variables");
      _num_variables = size_t(n);

      iss >> n;
      if (iss.fail() || n <= 0)
        throw Error("MaxSat::load: Bad number of clauses");
      num_clauses = size_t(n);

      spec = true;
    }

    // Clause
    else {
      if (!spec)
        throw Error("MaxSat::load: No p line");

      istringstream iss(line);
      int n;
      std::vector<int> clause;

      while (has_next_variable(iss)) {

        iss >> n;
        if (iss.fail())
          throw Error("MaxSat::load: Bad variable index");

        if (n == 0)
          break;

        int v = (n > 0) ? n : -n;
        if (size_t(v) > _num_variables)
          throw Error("MaxSat::load: Variable index overflow");

        clause.push_back(n);
      }

      if (n != 0)
        throw Error("MaxSat::load: Clause not ended by 0");

      clause.shrink_to_fit();
      _expression.push_back(clause);
    }
  }

  if (_expression.size() != num_clauses)
    throw Error("MaxSat::load: Incoherent number of clauses");

  _expression.shrink_to_fit();
}


void
MaxSat::save(ostream& stream)
{
  stream
    << "c HNCO MaxSat::save" << endl
    << "p cnf " << _num_variables << " " << _expression.size() << endl;
  for (size_t i = 0; i < _expression.size(); i++) {
    for (size_t j = 0; j < _expression[i].size(); j++) {
      stream << _expression[i][j] << " ";
    }
    stream << 0 << endl;
  }
}


void
MaxSat::display(ostream& stream)
{
  for (size_t i = 0; i < _expression.size(); i++) {
    if (i > 0)
      stream << " ^ ";
    stream << "(";
    for (size_t j = 0; j < _expression[i].size(); j++) {
      if (j > 0)
        stream << " v ";
      int lit = _expression[i][j];
      if (lit < 0) {
        stream << "¬";
        stream << "x" << (-lit);
      } else
        stream << "x" << lit;
    }
    stream << ")";
  }
}


double
MaxSat::eval(const bit_vector_t& x)
{
  double result = 0;

  for (size_t i = 0; i < _expression.size(); i++) {
    for (size_t j = 0; j < _expression[i].size(); j++) {
      int lit = _expression[i][j];
      if ((lit > 0 && (x[lit - 1] == 1)) ||
          (lit < 0 && (x[-lit - 1] == 0))) {
        result++;
        break;
      }
    }
  }

  return result;
}
