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

#ifndef HNCO_FUNCTIONS_DECORATOR_H
#define HNCO_FUNCTIONS_DECORATOR_H

#include <iostream>

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


/// %Function decorator
class Decorator: public Function {

protected:

  /// Decorated function
  Function *_function;

public:

  /// Constructor
  Decorator(Function *function):
    _function(function)
  {}


  /** @name Display
   */
  ///@{

  /// Display
  void display(std::ostream& stream) const override { _function->display(stream); }

  /// Describe a bit vector
  void describe(const bit_vector_t& x, std::ostream& stream) override { _function->describe(x, stream); }

  ///@}

};


} // end of namespace function
} // end of namespace hnco


#endif
