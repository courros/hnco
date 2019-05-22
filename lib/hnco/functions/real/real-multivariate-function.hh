/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_REAL_REAL_MULTIVARIATE_FUNCTION_H
#define HNCO_FUNCTIONS_REAL_REAL_MULTIVARIATE_FUNCTION_H

#include <vector>

#include "fparser/fparser.hh"


namespace hnco {
namespace function {
namespace real {


  /// Real multivariate function
  class RealMultivariateFunction {

  public:

    /// Destructor
    virtual ~RealMultivariateFunction() {}

    /// Get the dimension of vectors
    virtual int get_dimension() = 0;

    /// Evaluate a real vector
    virtual double eval(const std::vector<double> x) = 0;

  };


  /// Parsed real multivariate function
  class ParsedRealMultivariateFunction:
    public RealMultivariateFunction {

    /// Function parser
    FunctionParser _fparser;

    /// Number of variables
    int _num_variables = 0;

  public:

    /** Constructor.

        \param expression Expression to parse
    */
    ParsedRealMultivariateFunction(std::string expression);

    /// Get the dimension of vectors
    int get_dimension() { return _num_variables; }

    /// Evaluate a real vector
    double eval(const std::vector<double> x);

  };


} // end of namespace real
} // end of namespace function
} // end of namespace hnco


#endif
