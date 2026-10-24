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

#ifndef HNCO_FUNCTIONS_COLLECTION_SYSTEM_BINARY_POLYNOMIAL_EQUATIONS_H
#define HNCO_FUNCTIONS_COLLECTION_SYSTEM_BINARY_POLYNOMIAL_EQUATIONS_H

#include "hnco/functions/function.hh"

namespace hnco {
namespace function {

/// System of binary polynomial equations
class SystemBinaryPolynomialEquations : public Function {
private:
  /// Number of variables
  int _num_variables;
  /// Equations
  std::vector<std::vector<std::vector<int>>> _equations;

public:
  /// Default constructor
  SystemBinaryPolynomialEquations():
    _num_variables(0) {}
  /**
   * Random instance.
   * @param n Number of variables
   * @param d Degree
   * @param t Maximum number of terms
   */
  void random(int n, int d, int t);
  /// Get bit vector size
  int get_bv_size() const override { return _num_variables; }

  /**
   * @name IO
   */
  ///@{
  /// Display the expression
  void display(std::ostream& stream) const override;
  /**
   * Load instance.
   *
   * Format:
   * - A line starting with # is a comment.
   * - An empty line is ignored.
   * - One polynomial per line.
   * - Terms are separated by +
   * - A term is the constant 1 or a product of variables.
   * - A product is a space separated list of variables
   *
   * @param path File path
   * @throw std::runtime_error
   */
  void load(std::string path);
  /**
   * Save instance.
   * @param path File path
   * @throw std::runtime_error
   */
  void save(std::string path) const;
  ///@}

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t& bv) override;
};

} // end of namespace function
} // end of namespace hnco

#endif
