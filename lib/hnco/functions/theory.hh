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

#ifndef HNCO_FUNCTIONS_THEORY_H
#define HNCO_FUNCTIONS_THEORY_H

#include "function.hh"


namespace hnco::function {

  /// OneMax
  class OneMax:
    public Function {

    /// Bit vector size
    size_t _bv_size;

  public:

    /// Constructor
    OneMax(int bv_size):
      _bv_size(bv_size) {}

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    size_t get_bv_size() { return _bv_size; }

    /** Get the global maximum.
        \return _bv_size */
    double get_maximum() { return _bv_size; }

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /** Check whether the function provides incremental evaluation.
        \return true
    */
    bool provides_incremental_evaluation() { return true; }

    ///@}


    /** @name Evaluation
     */
    ///@{

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /// Incremental evaluation
    double incremental_eval(const bit_vector_t& x, double v, const hnco::sparse_bit_vector_t& flipped_bits);

    ///@}

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

    /** Get the global maximum.
        \return _bv_size */
    double get_maximum() { return _bv_size; }

  };

  /// Needle in a haystack
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

    /** Get the global maximum.
        \return 1 */
    double get_maximum() { return 1; }

  };

  /// Hierarchical if and only if
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

    /** Get the global maximum.
        \return (i + 1) * 2^i where 2^i = _bv_size */
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

    /** Get the global maximum.
        \return 2 * _bv_size */
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

    /** Get the global maximum.
        \return _bv_size + 2 */
    double get_maximum() { return _bv_size + 2; }

  };

} // end of namespace hnco::function


#endif
