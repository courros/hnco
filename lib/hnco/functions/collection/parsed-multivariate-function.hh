/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_COLLECTION_PARSED_MULTIVARIATE_FUNCTION_H
#define HNCO_FUNCTIONS_COLLECTION_PARSED_MULTIVARIATE_FUNCTION_H

#include <assert.h>

#include <iosfwd>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>            // std::remove_if

#include "hnco/util.hh"         // hnco::join

#include "fparser/fparser.hh"

namespace hnco {
namespace function {

/**
 * Parsed multivariate function.
 *
 * Uses the C++ library "Function Parser" (fparser):
 *
 * http://warp.povusers.org/FunctionParser/fparser.html
 *
 * @warning The function string syntax depends on the chosen parser.
 */
template<class Parser>
class ParsedMultivariateFunction {

  /// %Function parser
  Parser _fparser;

  /// Variable names
  std::vector<std::string> _variable_names;

  /// Expression
  std::string _expression;

public:

  /// Domain type
  using domain_type = typename Parser::value_type;

  /// Codomain type
  using codomain_type = typename Parser::value_type;

  /**
   * Constructor.
   * @param expression Expression to parse
   */
  ParsedMultivariateFunction(std::string expression)
    : _expression(expression)
  {}

  /// Add a constant to the parser
  bool add_constant(std::string name, domain_type value) { return _fparser.AddConstant(name, value); }

  /**
   * Parse the expression.
   */
  void parse() {
    int position = _fparser.ParseAndDeduceVariables(_expression, _variable_names);
    if (position != -1) {
      std::ostringstream stream;
      stream
        << "ParsedMultivariateFunction::ParsedMultivariateFunction: " << _fparser.ErrorMsg()
        << " at position: " << position
        << " in _expression: " << _expression;
      throw std::runtime_error(stream.str());
    }
    _fparser.Optimize();
  }

  /// Display the problem
  void display(std::ostream& stream) const {
    stream << "ParsedMultivariateFunction:" << std::endl;
    stream << "Variables: " << hnco::join(begin(_variable_names), end(_variable_names), ", ") << std::endl;
    stream << "Expression:" << std::endl;
    std::string str = _expression;
    str.erase(std::remove_if(str.begin(), str.end(),
                             [](unsigned char c) { return std::isspace(c); }),
              str.end());
    stream << str << std::endl;
  }

  /// Evaluate
  codomain_type evaluate(const std::vector<domain_type>& x) { return _fparser.Eval(x.data()); }

  /// Describe a solution
  void describe(const std::vector<domain_type>& x, std::ostream& stream) {
    assert(x.size() == _variable_names.size());
    for (std::size_t i = 0; i < x.size(); i++)
      stream << _variable_names[i] << " = " << x[i] << std::endl;
  }

  /// Get the number of variables
  int get_num_variables() { return _variable_names.size(); }

  /// Get variable names
  const std::vector<std::string>& get_variable_names() { return _variable_names; }

};


} // end of namespace function
} // end of namespace hnco


#endif
