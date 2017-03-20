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

#ifndef HNCO_FUNCTIONS_JUMP_H
#define HNCO_FUNCTIONS_JUMP_H

#include "function.hh"


namespace hnco {
namespace function {


  /// Jump function
  class Jump:
    public Function {

    /// Bit vector size
    size_t _bv_size;

    /// Gap
    int _gap;

  public:

    /// Constructor
    Jump(int bv_size, int gap);

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /// Get the global maximum
    double get_maximum() { return _bv_size; }

  };


  /** Deceptive jump function.

      This is a jump function with a deceptive gap as defined in
      "Analyzing evolutionary algorithms" by Thomas Jansen, where it
      is called Jump_k. Algorithms in the neighborhood of the
      maximizer (which is the all one bit vector) are taken away from
      it.
  */
  class DeceptiveJump:
    public Function {

    /// Bit vector size
    size_t _bv_size;

    /// Gap
    int _gap;

  public:

    /// Constructor
    DeceptiveJump(int bv_size, int gap);

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /// Get the global maximum
    double get_maximum() { return _bv_size + _gap; }

  };


} // end of namespace function
} // end of namespace hnco


#endif
