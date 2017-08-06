/* Copyright (C) 2016, 2017 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_DECORATORS_FUNCTION_DECORATOR_H
#define HNCO_FUNCTIONS_DECORATORS_FUNCTION_DECORATOR_H

#include <assert.h>

#include <iostream>
#include <unordered_map>

#include "hnco/map.hh"

#include "hnco/functions/function.hh"


namespace hnco::function {

  /// Function decorator
  class FunctionDecorator:
    public Function {

  protected:

    /// Decorated function
    Function *_function;

  public:

    /// Constructor
    FunctionDecorator(Function *function):
      _function(function) {}

  };

} // end of namespace hnco::function


#endif
