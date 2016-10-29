/* Copyright (C) 2016 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_THEORY_H
#define HNCO_FUNCTIONS_THEORY_H

#include "function.hh"


namespace hnco {
namespace function {


  /// OneMax
  class OneMax:
    public Function {

    /// Bit vector size
    size_t _bv_size;

  public:

    /// Constructor
    OneMax(int bv_size):
      _bv_size(bv_size) {}

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


  /// Leading ones
  class LeadingOnes:
    public Function {

    /// Bit vector size
    size_t _bv_size;

  public:

    /// Constructor
    LeadingOnes(int bv_size):
      _bv_size(bv_size) {}

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

  /// Needle
  class Needle:
    public Function {

    /// Bit vector size
    size_t _bv_size;

  public:

    /// Constructor
    Needle(int bv_size):
      _bv_size(bv_size) {}

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /// Get the global maximum
    double get_maximum() { return 1; }

  };

  /// Hiff
  class Hiff:
    public Function {

    /// Bit vector size
    size_t _bv_size;

    /// Tree depth
    size_t _depth;

  public:

    /// Constructor
    Hiff(int bv_size);

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /// Get the global maximum
    double get_maximum() { return (_depth + 1) * _bv_size; }

  };


  /// Ridge
  class Ridge:
    public Function {

    /// Bit vector size
    size_t _bv_size;

  public:

    /// Constructor
    Ridge(int bv_size):
      _bv_size(bv_size) {}

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /// Get the global maximum
    double get_maximum() { return 2 * _bv_size; }

  };


  /// Plateau
  class Plateau:
    public Function {

    /// Bit vector size
    size_t _bv_size;

  public:

    /// Constructor
    Plateau(int bv_size):
      _bv_size(bv_size) {}

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /// Get the global maximum
    double get_maximum() { return _bv_size + 2; }

  };


} // end of namespace function
} // end of namespace hnco


#endif
