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
  /// Term type
  using term_t = std::vector<int>;
  /// Equation type
  using equation_t = std::vector<term_t>;
  /// System type
  using system_t = std::vector<equation_t>;
  /// System
  system_t _system;
  /// Variable names
  std::vector<std::string> _variable_names;
  /// Is an equation satisfied
  bit_vector_t _is_satisfied;

  /// Get the degree of the system
  int get_degree() const;

public:
  /// Default constructor
  SystemBinaryPolynomialEquations() {}
  /**
   * Random instance.
   * @param num_variables Number of variables
   * @param num_equations Number of equations
   * @param degree Degree
   * @param num_terms_max Maximum number of terms
   * @pre num_terms_max <= C(num_variables, 0) + C(num_variables, 1) + ... + C(num_variables, degree)
   */
  void random(int num_variables, int num_equations, int degree, int num_terms_max);
  /// Get bit vector size
  int get_bv_size() const override { return _variable_names.size(); }

  /**
   * @name IO
   */
  ///@{
  /// Display the expression
  void display(std::ostream& stream) const override;
  /// Describe a bit vector
  void describe(const bit_vector_t& bv, std::ostream& stream) override;
  /**
   * Load instance.
   *
   * Format:
   * - The first line contains the ordered list of variables. The system follows.
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
