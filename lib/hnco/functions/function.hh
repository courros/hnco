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

#ifndef HNCO_FUNCTIONS_FUNCTION_H
#define HNCO_FUNCTIONS_FUNCTION_H

#include <iostream>

#include "hnco/bit-vector.hh"
#include "hnco/exception.hh"


namespace hnco {
/// All about fitness functions to be maximized
namespace function {


  /// Function
  class Function {

  protected:

  public:

    /// Destructor
    virtual ~Function() {}

    /// Get bit vector size
    virtual size_t get_bv_size() = 0;

    /// Evaluate a bit vector
    virtual double eval(const bit_vector_t&) = 0;

    /// Check for a known maximum.
    virtual bool has_known_maximum() { return false; }

    /** Get the global maximum.
        \throw Error */
    virtual double get_maximum() { throw exception::Error("Unknown maximum"); }

    /// Display
    virtual void display(std::ostream& stream) {}

    /// Describe a bit vector
    virtual void describe(const bit_vector_t& x, std::ostream& stream) {
      bv_display(x, stream);
      stream << std::endl;
    }

  };


} // end of namespace function
} // end of namespace hnco


#endif
