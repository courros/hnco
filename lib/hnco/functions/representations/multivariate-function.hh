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

#ifndef HNCO_FUNCTIONS_REPRESENTATIONS_MULTIVARIATE_FUNCTION_H
#define HNCO_FUNCTIONS_REPRESENTATIONS_MULTIVARIATE_FUNCTION_H

#include <vector>
#include <iostream>
#include <sstream>
#include <string>

#include "fparser/fparser.hh"


namespace hnco {
namespace function {
namespace representation {


/// Multivariate function
template<class T>
class MultivariateFunction {
public:

  /// Type of value
  typedef T value_type;

  /// Destructor
  virtual ~MultivariateFunction() {}

  /// Get the number of variables
  virtual int get_num_variables() = 0;

  /// Evaluate
  virtual T evaluate(const std::vector<value_type>& x) = 0;
};


/** Parsed multivariate function.

    Uses the C++ library "Function Parser" (fparser):

    http://warp.povusers.org/FunctionParser/fparser.html

    \warning The function string syntax depends on the chosen parser.

*/
template<class Parser, class T>
class ParsedMultivariateFunction:
    public MultivariateFunction<T> {

  /// Function parser
  Parser _fparser;

  /// Number of variables
  int _num_variables = 0;

public:

  /** Constructor.

      \param expression Expression to parse
  */
  ParsedMultivariateFunction(std::string expression)
  {
    int position = _fparser.ParseAndDeduceVariables(expression, &_num_variables);
    if (position != -1) {
      std::ostringstream stream;
      stream
        << "ParsedMultivariateFunction::ParsedMultivariateFunction: " << _fparser.ErrorMsg()
        << " at position: " << position
        << " in expression: " << expression;
      throw exception::Error(stream.str());
    }
    _fparser.Optimize();
  }

  /// Get the number of variables
  int get_num_variables() { return _num_variables; }

  /// Evaluate
  T evaluate(const std::vector<T>& x) { return _fparser.Eval(x.data()); }

};


} // end of namespace representation
} // end of namespace function
} // end of namespace hnco


#endif
