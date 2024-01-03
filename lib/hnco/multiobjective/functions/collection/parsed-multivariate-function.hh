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

#ifndef HNCO_MULTIOBJECTIVE_FUNCTIONS_COLLECTION_PARSED_MULTIVARIATE_FUNCTION_H
#define HNCO_MULTIOBJECTIVE_FUNCTIONS_COLLECTION_PARSED_MULTIVARIATE_FUNCTION_H

#include <assert.h>

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>            // std::sort, std::remove_if

#include "hnco/util.hh"         // hnco::join

#include "fparser/fparser.hh"


namespace hnco {
namespace multiobjective {
namespace function {


/**
 * Parsed multivariate function.
 *
 * Uses the C++ library "Function Parser" (fparser):
 *
 * http://warp.povusers.org/FunctionParser/fparser.html
 *
 * \warning The function string syntax depends on the chosen parser.
 */
template<class Parser>
class ParsedMultivariateFunction {

public:

  /// Domain type
  using domain_type = typename Parser::value_type;

  /// Codomain type
  using codomain_type = domain_type;

private:

  /// Expressions
  std::vector<std::string> _expressions;

  /// %Function parsers
  std::vector<Parser> _parsers;

  /**
   * Names.
   *
   * Indexed by parser then variable. Then, _names[i][j] is the name
   * of the jth variable of the ith parser.
   */
  std::vector<std::vector<std::string>> _names;

  /**
   * Variables.
   *
   * Indexed by parser then variable. Then, _variables[i][j] is the
   * value of the jth variable of the ith parser.
   */
  std::vector<std::vector<domain_type>> _variables;

  /**
   * Indices.
   *
   * Indexed by parser then variable. Then, _indices[i][j] is the
   * index in the vector to evaluate of the jth variable of the ith
   * parser.
   */
  std::vector<std::vector<int>> _indices;

  /**
   * Ordered variable names.
   *
   * As expected by evaluate().
   */
  std::vector<std::string> _ordered_names;

public:

  /**
   * Constructor.
   *
   * An expression is a list of sub expressions separated by double
   * colons (::). Each sub expression defines a multivariate function.
   *
   * \param expression Expression to parse
   */
  ParsedMultivariateFunction(std::string expression) {
    // Split expression into sub expressions
    const std::string delimiter = "::";
    auto start = 0U;
    auto stop = expression.find(delimiter);
    while (stop != std::string::npos) {
      _expressions.push_back(expression.substr(start, stop - start));
      start = stop + delimiter.length();
      stop = expression.find(delimiter, start);
    }
    _expressions.push_back(expression.substr(start));

    _parsers.resize(_expressions.size());
    _names.resize(_expressions.size());
    _variables.resize(_expressions.size());
  }

  /// Add a constant to the parsers
  void add_constant(std::string name, domain_type value) {
    for (auto& parser : _parsers)
      parser.AddConstant(name, value);
  }

  /**
   * Parse the expression.
   */
  void parse() {
    // Init parsers
    for (size_t i = 0; i < _parsers.size(); i++) {
      int position = _parsers[i].ParseAndDeduceVariables(_expressions[i], _names[i]);
      if (position != -1) {
        std::ostringstream stream;
        stream
          << "ParsedMultivariateFunction::ParsedMultivariateFunction: " << _parsers[i].ErrorMsg()
          << " at position: " << position
          << " in expression: " << _expressions[i];
        throw std::runtime_error(stream.str());
      }
      _parsers[i].Optimize();
      _variables[i].resize(_names[i].size());
    }

    // Collect all names into a set
    std::unordered_set<std::string> name_set;
    for (size_t i = 0; i < _names.size(); i++) {
      for (const auto& name : _names[i])
        name_set.insert(name);
    }

    // Order names
    for (const auto& name : name_set)
      _ordered_names.push_back(name);
    std::sort(begin(_ordered_names), end(_ordered_names));

    // Compute reverse hash
    std::unordered_map<std::string, int> index_of;
    for (std::size_t i = 0; i < _ordered_names.size(); i++)
      index_of[_ordered_names[i]] = i;

    // Compute indices
    _indices.resize(_expressions.size());
    for (size_t i = 0; i < _indices.size(); i++) {
      _indices[i].resize(_names[i].size());
      for (size_t j = 0; j < _names[i].size(); j++) {
        assert(index_of.find(_names[i][j]) != index_of.end());
        _indices[i][j] = index_of[_names[i][j]];
      }
    }
  }

  /// Get the number of variables
  int get_num_variables() const { return _ordered_names.size(); }

  /// Get output size (number of objectives)
  int get_output_size() const { return _parsers.size(); }

  /// Evaluate
  void evaluate(const std::vector<domain_type>& xs, std::vector<codomain_type>& values) {
    assert(int(xs.size()) == get_num_variables());
    assert(values.size() == _parsers.size());

    for (size_t i = 0; i < values.size(); i++) {
      auto& vars = _variables[i];
      auto& lut = _indices[i];
      for (size_t j = 0; j < vars.size(); j++) {
        assert(is_in_range(lut[j], xs.size()));
        vars[j] = xs[lut[j]];
      }
      values[i] = _parsers[i].Eval(vars.data());
    }
  }

  /// Display the problem
  void display(std::ostream& stream) const {
    stream << "ParsedMultivariateFunction:" << std::endl;
    stream << "Variables: " << hnco::join(_ordered_names.begin(), _ordered_names.end(), ", ") << std::endl;
    stream << "Objectives:" << std::endl;
    for (size_t i = 0; i < _parsers.size(); i++) {
      std::string str = _expressions[i];
      str.erase(std::remove_if(str.begin(), str.end(),
                               [](unsigned char c) { return std::isspace(c); }),
                str.end());
      stream << i << ": " << hnco::join(_names[i].begin(), _names[i].end(), ", ") << " -> " << str << std::endl;
    }
  }

  /// Describe a solution
  void describe(const std::vector<domain_type>& xs, std::ostream& stream) {
    assert(int(xs.size()) == get_num_variables());

    for (size_t i = 0; i < _ordered_names.size(); i++)
      stream << _ordered_names[i] << " = " << xs[i] << std::endl;
  }

  /// Get variable names
  const std::vector<std::string>& get_variable_names() { return _ordered_names; }

};


} // end of namespace function
} // end of namespace multiobjective
} // end of namespace hnco


#endif
