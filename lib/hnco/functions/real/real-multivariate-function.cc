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

#include "hnco/exception.hh"

#include "real-multivariate-function.hh"


using namespace hnco;
using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::function::real;


ParsedRealMultivariateFunction::ParsedRealMultivariateFunction(std::string expression)
{
  int result = _fparser.ParseAndDeduceVariables(expression, &_num_variables);
  if (result != -1) {
    std::ostringstream stream;
    stream
      << "ParsedRealMultivariateFunction::ParsedRealMultivariateFunction: " << _fparser.ErrorMsg()
      << " at position: " << result
      << " in expression: " << expression;
    throw Error(stream.str());
  }
}


double
ParsedRealMultivariateFunction::evaluate(const std::vector<double> x)
{
  return _fparser.Eval(x.data());
}
