/* Copyright (C) 2016, 2017 Arnaud Berny

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

#include "hnco/exception.hh"
#include "hnco/random.hh"

#include "qubo.hh"


using namespace hnco::random;
using namespace hnco::function;
using namespace hnco::exception;


void
Qubo::load(std::istream& stream)
{
  bool spec = false;
  int dimension = 0;
  int num_diagonal_elements = 0;
  int num_other_elements = 0;
  int count_diagonal_elements = 0;
  int count_other_elements = 0;

  while (!stream.eof()) {
    std::string line;
    getline(stream, line);

    if (line.empty())
      continue;

    // Comment
    if (line[0] == 'c')
      continue;

    // Spec
    if (line[0] == 'p') {

      if (spec)
        throw Error("Qubo::load: More than one p line");

      std::istringstream iss(line);
      std::string token;
      int n;

      iss >> token;
      if (iss.fail() || token != "p")
        throw Error("Qubo::load: Missing p line");

      iss >> token;
      if (iss.fail() || token != "qubo")
        throw Error("Qubo::load: p line: Missing token qubo");

      iss >> n;
      if (iss.fail() || n != 0)
        throw Error("Qubo::load: p line: Missing constant 0");

      iss >> dimension;
      if (iss.fail() || dimension <= 0)
        throw Error("Qubo::load: p line: Bad dimension");

      iss >> num_diagonal_elements;
      if (iss.fail() || num_diagonal_elements < 0)
        throw Error("Qubo::load: p line: Bad num_diagonal_elements");

      iss >> num_other_elements;
      if (iss.fail() || num_other_elements < 0)
        throw Error("Qubo::load: p line: Bad num_other_elements");

      _q = std::vector<std::vector<double> >(dimension, std::vector<double>(dimension, 0.0));

      spec = true;
    }

    // Element
    else {
      if (!spec)
        throw Error("Qubo::load: No p line yet");

      assert(dimension > 0);

      std::istringstream iss(line);
      int i, j;
      float value;

      iss >> i;
      if (iss.fail() || i < 0 || !(i < dimension))
        throw Error("Qubo::load: Bad line index");

      iss >> j;
      if (iss.fail() || j < 0 || !(j < dimension))
        throw Error("Qubo::load: Bad column index");

      if (i > j)
        throw Error("Qubo::load: matrix must be upper triangular");

      iss >> value;
      if (iss.fail() || (value == 0))
        throw Error("Qubo::load: Bad element value");

      _q[i][j] = -value;

      if (i == j)
        count_diagonal_elements++;
      else
        count_other_elements++;
    }
  }

  if (count_other_elements != num_other_elements)
    throw Error("Qubo::load: Not enough off diagonal elements");
  if (count_diagonal_elements != num_diagonal_elements)
    throw Error("Qubo::load: Not enough diagonal elements");

}


double
Qubo::eval(const bit_vector_t& x)
{
  assert(x.size() == _q.size());

  const size_t dimension = _q.size();

  double result = 0;

  for (size_t i = 0; i < dimension; i++) {
    if (!x[i])
      continue;
    auto line = _q[i];
    result += line[i];
    for (size_t j = i + 1; j < dimension; j++)
      if (x[j])
        result += line[j];
  }

  return result;
}
