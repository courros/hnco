/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#include "hnco/util.hh"         // hnco::join

#include "fparser/fparser.hh"

namespace hnco {
namespace multiobjective {
namespace function {

/** Parsed multivariate function.

    Uses the C++ library "Function Parser" (fparser):

    http://warp.povusers.org/FunctionParser/fparser.html

    \warning The function string syntax depends on the chosen parser.

*/
template<class Parser>
class ParsedMultivariateFunction {

public:

  /// Domain type
  using domain_type = typename Parser::value_type;

  /// Codomain type
  using codomain_type = typename Parser::value_type;

private:

  /// Expressions
  std::vector<std::string> _expressions;

  /// %Function parsers
  std::vector<Parser> _parsers;

  /** Variable names.

      Indexed by parser index.
  */
  std::vector<std::vector<std::string>> _names;

  /** Variables.

      Indexed by parser index.
  */
  std::vector<std::vector<domain_type>> _variables;

  /** Lookup tables.

      Indexed by parser index.
  */
  std::vector<std::vector<int>> _lookup_tables;

  /** Index of variable.

      In the vector of variables to be evaluated.
  */
  std::unordered_map<std::string, int> _index_of;

public:

  /** Constructor.

      An expression is a list of sub expressions separated by double
      colons (::). Each sub expression defines a multivariate
      function.

      \param expression Expression to parse
  */
  ParsedMultivariateFunction(std::string expression)
  {
    // Split expression into sub expressions
    std::string delimiter = "::";
    auto start = 0U;
    auto end = expression.find(delimiter);
    while (end != std::string::npos) {
      _expressions.push_back(expression.substr(start, end - start));
      start = end + delimiter.length();
      end = expression.find(delimiter, start);
    }
    _expressions.push_back(expression.substr(start));

    // Init parsers
    _parsers.resize(_expressions.size());
    _names.resize(_expressions.size());
    _variables.resize(_expressions.size());
    for (size_t i = 0; i < _parsers.size(); i++) {
      int position = _parsers[i].ParseAndDeduceVariables(_expressions[i], _names[i]);
      if (position != -1) {
        std::ostringstream stream;
        stream
          << "ParsedMultivariateFunction::ParsedMultivariateFunction: " << _parsers[i].ErrorMsg()
          << " at position: " << position
          << " in _expression: " << _expressions[i];
        throw std::runtime_error(stream.str());
      }
      _parsers[i].Optimize();
      _variables[i].resize(_names[i].size());
    }

    // All names
    std::unordered_set<std::string> all_names;
    for (size_t i = 0; i < _names.size(); i++) {
      for (const auto& name : _names[i])
        all_names.insert(name);
    }

    int index = 0;
    for (const auto& name : all_names)
      _index_of[name] = index++;

    // build lookup tables
    _lookup_tables.resize(_expressions.size());
    for (size_t i = 0; i < _lookup_tables.size(); i++) {
      _lookup_tables[i].resize(_names[i].size());
      for (size_t j = 0; j < _names[i].size(); j++) {
        assert(_index_of.find(_names[i][j]) != _index_of.end());

        _lookup_tables[i][j] = _index_of[_names[i][j]];
      }
    }
  }

  /// Get the number of variables
  int get_num_variables() const { return _index_of.size(); }

  /// Get output size (number of objectives)
  int get_output_size() const { return _parsers.size(); }

  /// Evaluate
  void evaluate(const std::vector<domain_type>& x, std::vector<codomain_type>& values) {
    assert(int(x.size()) == get_num_variables());
    assert(values.size() == _parsers.size());

    for (size_t i = 0; i < values.size(); i++) {
      auto& vars = _variables[i];
      auto& lut = _lookup_tables[i];
      for (size_t j = 0; j < vars.size(); j++) {
        assert(is_in_range(lut[j], x.size()));
        vars[j] = x[lut[j]];
      }
      values[i] = _parsers[i].Eval(vars.data());
    }
  }

  /// Display the problem
  void display(std::ostream& stream) const {
    stream << "ParsedMultivariateFunction:" << std::endl;
    std::vector<std::string> vars(_index_of.size());
    for (const auto& kv : _index_of)
      vars[kv.second] = kv.first;
    stream << "Variables: " << hnco::join(vars.begin(), vars.end(), ", ") << std::endl;
    stream << "Objectives:" << std::endl;
    for (size_t i = 0; i < _parsers.size(); i++)
      stream << hnco::join(_names[i].begin(), _names[i].end(), ", ") << " -> " << _expressions[i] << std::endl;
  }

  /// Describe a solution
  void describe(const std::vector<domain_type>& x, std::ostream& stream) {
    assert(int(x.size()) == get_num_variables());

    for (const auto& kv : _index_of) {
      assert(is_in_range(kv.second, x.size()));
      stream << kv.first << " = " << x[kv.second] << std::endl;
    }
  }

};


} // end of namespace function
} // end of namespace multiobjective
} // end of namespace hnco


#endif
