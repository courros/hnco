/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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
#include <cstdlib>              // std::abs

#include <iostream>
#include <string>
#include <sstream>

#include "hnco/exception.hh"

#include "sudoku.hh"

using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::function::representation;

template<class T>
void print_board(const std::vector<std::vector<T>>& board, std::ostream& stream, bool pretty)
{
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      stream << board[i][j];
      if (pretty) {
        if (j == 2 || j == 5)
          stream << "|";
      }
    }
    stream << std::endl;
    if (pretty) {
      if (i == 2 || i == 5)
        stream << "---+---+---" << std::endl;
    }
  }
}

void
Sudoku::load(std::istream& stream)
{
  _num_variables = 0;
  int i = 0;
  while (!stream.eof()) {
    std::string line;
    getline(stream, line);
    if (line.size() < 9) {
      std::ostringstream oss;
      oss << "Sudoku::load: line " << i << " too short" << std::endl;
      throw Error(oss.str());
    }
    for (int j = 0; j < 9; j++) {
      char c = line[j];
      if (!(c == '.' || (isdigit(c) && c != '0')))
        throw Error("Sudoku::load: all characters must be the dot . or a digit between 1 and 9 (inclusive)");
      _problem_instance[i][j] = c;
      if (c == '.')
        _num_variables++;
      else {
        assert(c - '0' > 0);
        _candidate[i][j] = c - '0';
      }
    }
    i++;
    if (i == 9)
      break;
  }
  if (i < 9)
    throw Error("Sudoku::load: at least 9 lines are needed");
}

void
Sudoku::save(std::ostream& stream)
{
  print_board(_problem_instance, stream, false);
}

void
Sudoku::display(std::ostream& stream)
{
  print_board(_problem_instance, stream, true);
}

void
Sudoku::write_variables(const std::vector<domain_type>& x)
{
  int index = 0;
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      if (_problem_instance[i][j] == '.')
        _candidate[i][j] = x[index++] + 1;
  assert(index == int(x.size()));
}

double
Sudoku::evaluate(const std::vector<domain_type>& x)
{
  assert(int(x.size()) == _num_variables);

  write_variables(x);

  int result = 0;

  // check lines
  for (int i = 0; i < 9; i++) {
    std::fill(begin(_counts), end(_counts), 0);
    for (int j = 0; j < 9; j++) {
      domain_type d = _candidate[i][j];
      assert(d > 0);
      assert(d < 10);
      _counts[d]++;
    }
    assert(_counts[0] == 0);
    for (int k = 1; k < 10; k++)
      result += std::abs(_counts[k] - 1);
  }

  // check columns
  for (int j = 0; j < 9; j++) {
    std::fill(begin(_counts), end(_counts), 0);
    for (int i = 0; i < 9; i++) {
      domain_type d = _candidate[i][j];
      assert(d > 0);
      assert(d < 10);
      _counts[d]++;
    }
    assert(_counts[0] == 0);
    for (int k = 1; k < 10; k++)
      if (_counts[k] != 1)
        result += std::abs(_counts[k] - 1);
  }

  // check blocks
  for (int a = 0; a < 9; a += 3)
    for (int b = 0; b < 9; b += 3) {
      std::fill(begin(_counts), end(_counts), 0);
      for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
          domain_type d = _candidate[a + i][b + j];
          assert(d > 0);
          assert(d < 10);
          _counts[d]++;
        }
      assert(_counts[0] == 0);
      for (int k = 1; k < 10; k++)
        if (_counts[k] != 1)
          result += std::abs(_counts[k] - 1);
    }

  return -result;
}

void
Sudoku::describe(const std::vector<domain_type>& x, std::ostream& stream)
{
  write_variables(x);
  print_board(_candidate, stream, true);
}
