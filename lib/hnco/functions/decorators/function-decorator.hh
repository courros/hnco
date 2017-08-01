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


    /// Get bit vector size
    size_t get_bv_size() { return _function->get_bv_size(); }

    /// Check for a known maximum.
    bool has_known_maximum() { return _function->has_known_maximum(); }

    /// Get the global maximum
    double get_maximum() { return _function->get_maximum(); }

    /// Check whether the function provides incremental evaluation
    bool provides_incremental_evaluation() { return _function->provides_incremental_evaluation(); }


    /// Incremental evaluation
    double delta(const bit_vector_t& x, double v, const hnco::sparse_bit_vector_t& flipped_bits) {
      return _function->delta(x, v, flipped_bits);
    }


    /// Display
    void display(std::ostream& stream) { _function->display(stream); }

    /// Describe
    void describe(const bit_vector_t& x, std::ostream& stream) { _function->describe(x, stream); }

  };

} // end of namespace hnco::function


#endif
