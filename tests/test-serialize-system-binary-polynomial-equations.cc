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

#include "hnco/functions/collection/system-binary-polynomial-equations.hh"
#include "hnco/random.hh"

#include <fstream>              // std::ifstream, std::ofstream

using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;

int main(int argc, char *argv[])
{
  Generator::set_seed();

  const int num_iterations = 10;
  const int num_evaluations = 10;
  const std::string path("test-serialize-system-binary-polynomial-equations.txt");

  std::uniform_int_distribution<int> dist(1, 100);

  for (int i = 0; i < num_iterations; i++) {

    const int num_variables = dist(Generator::engine);
    const int num_equations = dist(Generator::engine);
    const int degree        = dist(Generator::engine);
    const int num_terms_max = num_variables;

    SystemBinaryPolynomialEquations src;
    src.random(num_variables, num_equations, degree, num_terms_max);
    src.save(path);

    SystemBinaryPolynomialEquations dest;
    try {
      dest.load(path);
    }
    catch (const std::runtime_error& e) {
      std::cerr << "test-serialize-system-binary-polynomial-equations: std::runtime_error: " << e.what() << std::endl;
      exit(1);
    }

    if (src.get_bv_size() != dest.get_bv_size()) {
      std::cerr << "test-serialize-system-binary-polynomial-equations: different bv_size" << std::endl;
      exit(1);
    }

    bit_vector_t bv(num_variables);
    for (int j = 0; j < num_evaluations; j++) {
      bv_random(bv);
      if (src.evaluate(bv) != dest.evaluate(bv)) {
        std::cerr << "test-serialize-system-binary-polynomial-equations: different evaluations" << std::endl;
        exit(1);
      }
    }

  }

  return 0;
}
