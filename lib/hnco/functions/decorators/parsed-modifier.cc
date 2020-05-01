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

#include "hnco/exception.hh"

#include "parsed-modifier.hh"


using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::function::modifier;


ParsedModifier::ParsedModifier(Function *function, std::string expression):
  FunctionModifier(function)
{
  int result = _fparser.Parse(expression, "x");
  if (result != -1) {
    std::ostringstream stream;
    stream
      << "ParsedModifier::ParsedModifier: " << _fparser.ErrorMsg()
      << " at position: " << result
      << " in expression: " << expression;
    throw Error(stream.str());
  }
}


double
ParsedModifier::eval(const bit_vector_t& x)
{
  _values[0] = _function->eval(x);
  return _fparser.Eval(_values);
}
